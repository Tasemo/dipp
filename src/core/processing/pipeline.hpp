#pragma once

#include <model/context.hpp>
#include <tlx/counting_ptr.hpp>

namespace processing {

/**
 * @brief a processor is a generic interface that transforms a input to some output
 */
template <typename Input, typename Output>
class Processor {
 public:
  virtual ~Processor() = default;
  /**
   * @brief transforms the input to some output
   */
  virtual Output process(const model::Context &ctx, const Input &data) const = 0;
};

/**
 * @brief a pipeline is a processor, that also allows other processor to be attached, thus forming a pipeline
 */
template <typename Input, typename Output>
class Pipeline : public Processor<Input, Output> {
 private:
  virtual void inc_ref_counter() const {}
  virtual bool dec_ref_counter() const {
    return false;
  }

 public:
  virtual ~Pipeline() = default;

  /**
   * @brief adds the processor to the chain by returning a smart pointer to a object representing the transformation
   *
   * @note you can only add processors which accept the output of the current stage as input
   */
  template <typename NextOutput>
  auto add_next(const Processor<Output, NextOutput> &next) const {
    class ChainedStage : public Pipeline<Input, NextOutput>, private tlx::ReferenceCounter {
      friend class tlx::CountingPtr<ChainedStage>;

     private:
      const Pipeline<Input, Output> &_current;
      const Processor<Output, NextOutput> &_next;

      void inc_ref_counter() const override {
        inc_reference();
      }

      bool dec_ref_counter() const override {
        return dec_reference();
      }

     public:
      ChainedStage(const ChainedStage &) = delete;
      ChainedStage(ChainedStage &&) = delete;
      ChainedStage &operator=(const ChainedStage &) = delete;
      ChainedStage &operator=(ChainedStage &&) = delete;

      ChainedStage(const Pipeline<Input, Output> &current, const Processor<Output, NextOutput> &next)
          : _current(current), _next(next) {}

      ~ChainedStage() {
        if (_current.dec_ref_counter()) {
          delete &_current;
        }
      }

      NextOutput process(const model::Context &ctx, const Input &data) const override {
        auto result = _current.process(ctx, data);
        return _next.process(ctx, std::forward<Output>(result));
      }
    };
    inc_ref_counter();
    return tlx::make_counting<ChainedStage>(*this, next);
  }
};

}  // namespace processing
