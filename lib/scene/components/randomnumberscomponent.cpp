#include "randomnumberscomponent.hpp"
#include <lib/system/randomsystem.hpp>

#include <lib/include/liblog.hpp>
#include <lib/scene/scenenode.hpp>

namespace lib::scene
{
class RandomNumbersComponent::RandomNumbersComponentPrivate
{
public:
    constexpr RandomNumbersComponentPrivate(core::RandomSystem &random_system) noexcept
        : random_system_{random_system}
    {
        log_assert(std::addressof(random_system_) != nullptr, "nullptr RandomSystem received");
    }

    ~RandomNumbersComponentPrivate() = default;

    const str &name() const noexcept { return name_; }
    void setName(str name) { std::swap(name, name_); }

    core::RandomSystem &randomsystem() noexcept { return random_system_; }

private:
    core::RandomSystem &random_system_;
    str name_;
};

RandomNumbersComponent::RandomNumbersComponent() noexcept : priv_{nullptr} {}

RandomNumbersComponent::~RandomNumbersComponent() = default;

void RandomNumbersComponent::setName(str new_name)
{
    priv_->setName(std::move(new_name));
}

const str &RandomNumbersComponent::name() const noexcept
{
    return priv_->name();
}

u32 RandomNumbersComponent::getUInt(const size_type max, const size_type min) const
{
    DisplayLog::info("Asked for random number between ", min, " and ", max);

    log_assert(min != max, "The min and max parameters must be different");
    log_assert(max > min, "The max paramter must be greater than min");

    const auto next(priv_->randomsystem().getNext(name(), min, max));
    DisplayLog::info("Fetch next element from queue: ", next);
    const size_type generated(next % (max - min));
    DisplayLog::info("\tGot ", generated);
    DisplayLog::info("\tReturning ", min + generated);
    return min + generated;
}

void RandomNumbersComponent::onAttached()
{
    if (attachedNode())
    {
        priv_ = muptr<RandomNumbersComponentPrivate>(attachedNode()
                                                        ->randomSystem());
    }
}
} // namespace lib::scene
