#pragma once

enum BehaviorStatus
{
    BH_INVALID,
    BH_SUCCESS,
    BH_FAILURE,
    BH_RUNNING
};

class IBehaviorBase
{
public:
    BehaviorStatus m_BHStatus = BH_INVALID;

    virtual BehaviorStatus Update() = 0;
    virtual void onInitialize() {}
    virtual void onTerminate() {}

    BehaviorStatus tick()
    {
        if (m_BHStatus == BH_INVALID) {
            onInitialize();
            m_BHStatus = BH_RUNNING;
        }

        m_BHStatus = Update();

        if (m_BHStatus != BH_RUNNING)
            onTerminate();

        return m_BHStatus;
    }
};


class IBehavior : public IBehaviorBase
{
public:
    IBehavior* m_Parent = nullptr;
    IBehavior* m_Child = nullptr;
    IBehavior* m_Next = nullptr;

    IBehavior(IBehavior* parent) { m_Parent = parent; }

    void AddChild(IBehavior* newnode)
    {
        if (m_Child)
            newnode->m_Child = m_Child;

        m_Child = newnode;
    }
    void AddNext(IBehavior* newnode)
    {
        if (m_Next)
            newnode->m_Next = m_Next;

        m_Next = newnode;
    }
    IBehavior* FindPrev() 
    {
        if (m_Parent->m_Child == this)
            return nullptr;
        
        IBehavior* node = m_Parent->m_Child;

        while (node->m_Next) {
            if (node->m_Next == this)
                return node;

            node = node->m_Next;
        }
        
        return nullptr;
    }
    void RemoveChildAll()
    {
        while (m_Child) {
            IBehavior* node = m_Child;
            m_Child = node->m_Next;

            node->RemoveChildAll();
            delete node;
        }
    }
    void RemoveLink()
    {
        RemoveChildAll();

        if (m_Parent->m_Child == this) {
            m_Parent->m_Child = m_Next;            
        }
        else {
            IBehavior* prev = FindPrev();
            if (prev) {
                prev->m_Next = m_Next;
            }
        }
    }
    void RemoveNext() {
        IBehavior* n = m_Next;
        if (n) {
            m_Next = n->m_Next;
            delete n;
        }
    }
};


class BTSequence : public IBehavior
{
public:
    BTSequence(IBehavior* parent) : IBehavior(parent) {}

    virtual BehaviorStatus Update() override
    {
        BehaviorStatus s = BH_RUNNING;

        IBehavior* bh = m_Child;

        while (bh)
        {
            s = bh->tick();
            switch (s)
            {
            case BH_SUCCESS:
                bh = bh->m_Next;
                break;
            case BH_FAILURE:
                bh = nullptr;
                break;
            case BH_RUNNING:
                bh = nullptr;
                break;
            }
        }

        return s;
    }
};

class BTFallback : public IBehavior
{
public:

    virtual BehaviorStatus Update() override
    {
        BehaviorStatus s = BH_RUNNING;
        IBehavior* bh = m_Child;

        while (bh)
        {
            s = bh->tick();
            switch (s)
            {
            case BH_SUCCESS:
                 bh = nullptr;
                break;
            case BH_FAILURE:
                bh = bh->m_Next;
                break;
            case BH_RUNNING:
                bh = nullptr;
                break;
            }
        }

        return s;
    }
};
