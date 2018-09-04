// random_engine - random engine type ersure

#ifndef INCLUDED_RANDOM_ENGINE_HPP
#define INCLUDED_RANDOM_ENGINE_HPP

#include <cstddef>
#include <memory>
#include <random>
#include <utility>


namespace cxx
{
    namespace random_engine_detail
    {
        class eraser_base
        {
        public:
            virtual ~eraser_base() = default;
            virtual std::unique_ptr<eraser_base> clone() const = 0;

            virtual void uniform(float* buf, std::size_t count) = 0;
            virtual void uniform(double* buf, std::size_t count) = 0;
            virtual void normal(float* buf, std::size_t count) = 0;
            virtual void normal(double* buf, std::size_t count) = 0;
        };

        template<typename URNG>
        class eraser : public eraser_base
        {
        public:
            explicit eraser(URNG&& engine) noexcept
                : engine_{std::move(engine)}
            {
            }

            std::unique_ptr<eraser_base> clone() const override
            {
                return std::unique_ptr<eraser>{new eraser{*this}};
            }

            void uniform(float* buf, std::size_t count) override
            {
                generate(buf, count, std::uniform_real_distribution<float>{});
            }

            void uniform(double* buf, std::size_t count) override
            {
                generate(buf, count, std::uniform_real_distribution<double>{});
            }

            void normal(float* buf, std::size_t count) override
            {
                generate(buf, count, std::normal_distribution<float>{});
            }

            void normal(double* buf, std::size_t count) override
            {
                generate(buf, count, std::normal_distribution<double>{});
            }

            template<typename T, typename Distr>
            void generate(T* buf, std::size_t count, Distr distr)
            {
                for (std::size_t i = 0; i < count; i++) {
                    buf[i] = distr(engine_);
                }
            }

        private:
            URNG engine_;
        };
    }

    class random_engine
    {
    public:
        template<typename URNG>
        random_engine(URNG engine)
            : eraser_{new random_engine_detail::eraser<URNG>{std::move(engine)}}
        {
        }

        random_engine(random_engine const& other)
        {
            if (other.eraser_) {
                eraser_ = other.eraser_->clone();
            }
        }

        random_engine(random_engine&& other) noexcept
            : eraser_{std::move(other.eraser_)}
        {
        }

        random_engine& operator=(random_engine const& other)
        {
            random_engine{other}.swap(*this);
            return *this;
        }

        random_engine& operator=(random_engine&& other) noexcept
        {
            random_engine{std::move(other)}.swap(*this);
            return *this;
        }

        void swap(random_engine& other) noexcept
        {
            eraser_.swap(other.eraser_);
        }

        void uniform(float* buf, std::size_t count)
        {
            eraser_->uniform(buf, count);
        }

        void uniform(double* buf, std::size_t count)
        {
            eraser_->uniform(buf, count);
        }

        void normal(float* buf, std::size_t count)
        {
            eraser_->normal(buf, count);
        }

        void normal(double* buf, std::size_t count)
        {
            eraser_->normal(buf, count);
        }

    private:
        std::unique_ptr<random_engine_detail::eraser_base> eraser_;
    };
}

#endif
