#pragma once

#include <QtPlugin>

class StepInterface {
public:
    virtual void setStep(int) = 0;
    virtual int step() const = 0;
    virtual int stepCount() const = 0;
};

Q_DECLARE_INTERFACE(StepInterface, "stepinterface");

template<typename T, int STEPCOUNT = 10>
class StepTrait : public StepInterface {
public:
    void setStep(int step) override {
        m_step = step;
        static_cast<T*>(this)->update();
    }

    int step() const override { return m_step; }

    int stepCount() const override { return STEPCOUNT; }

private:
    int m_step = 0;
};
