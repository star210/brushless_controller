#pragma once

template<class T>
class Control {

  struct Step {

    enum class Type { FUNCTION, WAIT };
    Type type;

    union {
      bool(T::*noParam)();
      bool(T::*t_noParam)(float);
      bool(T::*t_intParam)(float, int);
      bool(T::*t_floatParam)(float, float);
    };
    enum class Param { NONE, TIME, TIME_INT, TIME_FLOAT };
    Param paramType;

    union {
      int argInt;
      float argFloat;
    };
  };


public:
  Control(T *obj):
    mObj(obj), mStepCounter(0) {

  }

  void add(bool(T::*func)()) {
    Step &step = mSteps[mStepCounter++];
    step.noParam = func;
    step.paramType = Step::Param::NONE;
    step.type = Step::Type::FUNCTION;
  }

  void add(bool(T::*func)(float)) {
    Step &step = mSteps[mStepCounter++];
    step.t_noParam = func;
    step.paramType = Step::Param::TIME;
    step.type = Step::Type::FUNCTION;
  }

  void add(bool(T::*func)(float, int), int value) {
    Step &step = mSteps[mStepCounter++];
    step.t_intParam = func;
    step.paramType = Step::Param::TIME_INT;
    step.argInt = value;
    step.type = Step::Type::FUNCTION;
  }

  void add(bool(T::*func)(float, float), float value) {
    Step &step = mSteps[mStepCounter++];
    step.t_floatParam = func;
    step.paramType = Step::Param::TIME_FLOAT;
    step.argFloat = value;
    step.type = Step::Type::FUNCTION;
  }

  void start() {
    mRunCounter = 0;
  }

  bool run(float currentTime) {

    if (mRunCounter == mStepCounter) {
      return false;
    }

    Step &step = mSteps[mRunCounter];

    bool result;

    switch (step.paramType) {
      case Step::Param::NONE:
        result = (mObj->*(step.noParam))();
        break;
      case Step::Param::TIME:
        result = (mObj->*(step.t_noParam))(currentTime);
        break;
      case Step::Param::TIME_INT:
        result = (mObj->*(step.t_intParam))(currentTime, step.argInt);
        break;
      case Step::Param::TIME_FLOAT:
        result = (mObj->*(step.t_floatParam))(currentTime, step.argFloat);
        break;
    }

    if (result) {
      ++mRunCounter;
    }

    return mRunCounter != mStepCounter;
  }

protected:
  T *mObj;

  uint8_t mStepCounter;
  Step mSteps[25];

  uint8_t mRunCounter;
};



