#include "CollideBvhTree.h"
#include <vector>
#include <algorithm>


CollideBvhTree::CollideBvhTree(int split_count) : split_count(split_count)
{

}

CollideBvhTree::~CollideBvhTree()
{
    clear();
}


void CollideBvhTree::insert(Collider* c)
{
    if (colliders.size() < ++colliders_top)
        colliders.push_back({ c, c->get_aabb() });
    else
        colliders[colliders_top - 1] = { c, c->get_aabb() };
}

void CollideBvhTree::build()
{
    root = new Node;
    root->l = 0;
    root->r = colliders_top - 1;
    root->father = nullptr;
    root->dep = 0;
    root->vertical = true;

    node_count = 0;
    std::vector<Node*> stk;
    stk.push_back(root);

    while (stk.size())
    {
        Node* cur = stk.back();
        int l = cur->l;
        int r = cur->r;
        stk.pop_back();
        node_count++;

        if (l <= r)
            cur->aabb = colliders[l].aabb;
        for (int i = l + 1; i <= r; i++)
            cur->aabb.unite(colliders[i].aabb);

        if (r - l + 1 >= split_count)
        {
            static auto cmp_vertical = [](const CollideAABB& l, const CollideAABB& r) {
                return l.aabb.cy() < r.aabb.cy();
            };
            static auto cmp_horizontal = [](const CollideAABB& l, const CollideAABB& r) {
                return l.aabb.cx() < r.aabb.cx();
            };
            if (cur->vertical)
                std::sort(colliders.begin() + l, colliders.begin() + r + 1, cmp_vertical);
            else
                std::sort(colliders.begin() + l, colliders.begin() + r + 1, cmp_horizontal);

            int mid = (l + r) / 2;

            cur->child[0] = new Node;
            cur->child[0]->dep = cur->dep + 1;
            cur->child[0]->father = cur;
            cur->child[0]->l = l;
            cur->child[0]->r = mid;
            cur->child[0]->vertical = !cur->vertical;
            stk.push_back(cur->child[0]);

            cur->child[1] = new Node;
            cur->child[1]->dep = cur->dep + 1;
            cur->child[1]->father = cur;
            cur->child[1]->l = mid + 1;
            cur->child[1]->r = r;
            cur->child[1]->vertical = !cur->vertical;
            stk.push_back(cur->child[1]);
        }
    }
}

int CollideBvhTree::test(Collider* c)
{
    int fail = 0;
    AABB aabb = c->get_aabb();

    std::vector<Node*> stk;
    stk.push_back(root);

    while (stk.size())
    {
        Node* cur = stk.back();
        stk.pop_back();

        if (cur->child[0])
        {
            if (aabb.test(cur->child[0]->aabb))
                stk.push_back(cur->child[0]);
            if (aabb.test(cur->child[1]->aabb))
                stk.push_back(cur->child[1]);
        }
        else
        {
            for (int i = cur->l; i <= cur->r; i++)
            {
                fail += !test_collide(c, colliders[i].collider);
            }
        }
    }

    return fail;
}

void CollideBvhTree::clear()
{
    if (!root)
        return;

    std::vector<Node*> stk;
    stk.push_back(root);
    while (stk.size())
    {
        Node* node = stk.back();
        stk.pop_back();
        for (int i = 0; i < 2; i++)
            if (node->child[i])
                stk.push_back(node->child[i]);
        delete node;
    }
    node_count = 1;
    colliders_top = 0;
}

int CollideBvhTree::get_node_count() const
{
    return node_count;
}

int CollideBvhTree::get_split_count() const
{
    return split_count;
}