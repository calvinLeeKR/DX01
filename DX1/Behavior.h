#pragma once

enum BehaviorStatus
{
    BH_INVALID,
    BH_SUCCESS,
    BH_FAILURE,
    BH_RUNNING
};

class IBehavior
{
protected:
    BehaviorStatus m_BHStatus = BH_INVALID;

public:
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


class CompositeBH : public IBehavior
{
protected:
    typedef std::vector<IBehavior*> IBehaviors;
    IBehaviors m_Children;

public:
    void AddChild(IBehavior* child) { m_Children.push_back(child); }
    void RemoveChild(IBehavior*);
    void ClearChildren();
};


class SequenceBH : public CompositeBH
{
    int  m_CurrentIdx = 0;
protected:
    virtual void onInitialize() override { m_CurrentIdx = 0; }

    virtual BehaviorStatus Update() override
    {
        BehaviorStatus r = BH_RUNNING;

        if (m_CurrentIdx < m_Children.size())
        {
            BehaviorStatus s = m_Children[m_CurrentIdx]->tick();
            
            switch (s)
            {
            case BH_SUCCESS: 
                ++m_CurrentIdx; 
                r = BH_RUNNING;
                break;
            case BH_FAILURE: 
                r = BH_FAILURE; 
                break;
            }
        }
        else {
            r = BH_SUCCESS;
        }

        return r;             
    }
};



class FallbackBH : public CompositeBH
{
    int  m_CurrentIdx = 0;
protected:
    virtual void onInitialize() override { m_CurrentIdx = 0; }

    virtual BehaviorStatus Update() override
    {
        if (m_CurrentIdx < m_Children.size())
        {
            BehaviorStatus s = m_Children[m_CurrentIdx]->tick();

            switch (s)
            {
            case BH_FAILURE: ++m_CurrentIdx; break;
            case BH_SUCCESS: m_BHStatus = BH_SUCCESS; break;
            }
        }
        else {
            m_BHStatus = BH_FAILURE;
        }

        return m_BHStatus;
    }
};


