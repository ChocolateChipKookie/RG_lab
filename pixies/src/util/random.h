#pragma once
#include <random>
#include <chrono>
#include <set>

namespace kki
{
	class Random
	{
    private:
		std::default_random_engine engine_;

	public:
		Random(unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count()) : engine_(seed) {}

		std::default_random_engine& get_engine()
		{
			return engine_;
		}

		///Returns a random element from a sequence container
		///Prequisites, container must have at(int) and size() functions
		template<typename  container_t>
		auto& randomElement(container_t& container)
		{
			return container.at(random_index(container.size()));
		}

		///Returns random boolean
		bool randomBool()
		{
			return static_cast<bool>(std::uniform_int_distribution<unsigned>(1)(engine_));
		}

		///Returns random integer of type integer_t in the range [0, max]
		template<typename integer_t = int>
		integer_t randomInt()
		{
			return std::uniform_int_distribution<integer_t>()(engine_);
		}
		
		
		///Returns random integer of type integer_t in the range [0, max]
		template<typename integer_t = int>
		integer_t randomInt(integer_t max)
		{
			return std::uniform_int_distribution<integer_t>(0, max)(engine_);
		}

		///Returns random integer of type integer_t in the range [min, max]
		template<typename integer_t = int>
		integer_t randomInt(integer_t min, integer_t max)
		{
			return std::uniform_int_distribution<integer_t>(min, max)(engine_);
		}

		///Returns random integer of type integer_t in the range [min, max>
		template<typename integer_t = size_t>
		integer_t randomIndex(integer_t max)
		{
			return std::uniform_int_distribution<integer_t>(0, max - 1)(engine_);
		}

		///Returns random integer of type integer_t in the range [min, max>
		template<typename integer_t = size_t>
		integer_t randomIndex(integer_t min, integer_t max)
		{
			return std::uniform_int_distribution<integer_t>(min, max - 1)(engine_);
		}

		///Returns random double of type real_t in the range [0, 1]
		template<typename real_t = double>
		real_t randomReal()
		{
			return std::uniform_real_distribution<real_t>()(engine_);
		}

		///Returns random double of type real_t in the range [0, max]
		template<typename real_t = double>
		real_t randomReal(real_t max)
		{
			return std::uniform_real_distribution<real_t>(0, max)(engine_);
		}

		///Returns random double of type real_t in the range [min, max]
		template<typename real_t = double>
		real_t randomReal(real_t min, real_t max)
		{
			return std::uniform_real_distribution<real_t>(min, max)(engine_);
		}

		///Returns random double of type real_t following the normal distribution with given mean and sigma
		template<typename real_t = double>
		real_t randomGaussian(real_t mean = 0, real_t sigma = 1)
		{
			return std::normal_distribution<real_t>(mean, sigma)(engine_);
		}

		///Returns random value of type value_t following the given distribution
		template<typename value_t, typename distribution>
		value_t randomValue(distribution dist)
		{
			return dist(engine_);
		}

		///Returns random lowercase letter
		char randomLowercase()
		{
			return std::uniform_int_distribution<int>('a', 'z')(engine_);
		}

		///Returns random uppercase letter
		char randomUppercase()
		{
			return std::uniform_int_distribution<int>('A', 'Z')(engine_);
		}

		///Returns random char digit
		char randomDigit()
		{
			return std::uniform_int_distribution<int>('0', '9')(engine_);
		}

		///Returns random digit, uppercase or lowercase letter
		char randomAlnum()
		{
			const int dec = std::uniform_int_distribution<int>(0, 61)(engine_);
			if (dec < 10)
			{
				return dec + 48;
			}
			if (dec < 36)
			{
				return dec + 55;
			}
			return dec + 61;
		}

		///Returns random char
		template<typename char_t = char>
		char_t randomChar()
		{
			return std::uniform_int_distribution<int>(std::numeric_limits<char_t>::min(), std::numeric_limits<char_t>::max())(engine_);
		}
		
		///Returns random char
		char randomChar(char min, char max)
		{
			return std::uniform_int_distribution<int>(min, max)(engine_);
		}

        ///Populates container with random floating point numbers
        template<typename T_real, typename T_container>
        inline T_container& populateReal(T_container& c){
            return populateReal<T_real>(c, static_cast<T_real>(0.), static_cast<T_real>(1.));
        }
        template<typename T_real, typename T_container>
        inline T_container& populateReal(T_container& c, T_real max){
            return populateReal<T_real>(c, static_cast<T_real>(0.), max);
        }
		///Populates container with random floating point numbers
        template<typename T_real, typename T_container>
		T_container& populateReal(T_container& c, T_real min, T_real max){
            static_assert(std::is_floating_point<T_real>::value, "template argument must be an floating point type");
            for (auto& v : c){
		        v = randomReal<T_real>(min, max);
		    }
            return c;
		}

        ///Populates container with random integral numbers
        template<typename T_int, typename T_container>
        inline T_container& populateIntegral(T_container& c){
            static_assert(std::is_integral<T_int>::value, "template argument must be an integral type");
            for (auto& v : c){
                v = randomInt<T_int>();
            }
            return c;
        }
        ///Populates container with random integral numbers
        template<typename T_int, typename T_container>
        inline T_container& populateIntegral(T_container& c, T_int max){
            return populateIntegral(c, static_cast<T_int>(0), max);
        }
        ///Populates container with random integral numbers
        template<typename T_int, typename T_container>
        inline T_container& populateIntegral(T_container& c, T_int min, T_int max){
            static_assert(std::is_integral<T_int>::value, "template argument must be an integral type");
            for (auto& v : c){
                v = randomInt<T_int>(min, max);
            }
            return c;
        }

        ///Populates container with random indeces
        template<typename T_ind, typename T_container>
        inline T_container& populateIndeces(T_container& c, T_ind max){
            return populateIndeces(c, static_cast<T_ind>(0), max);
        }
        ///Populates container with random indeces
        template<typename T_ind, typename T_container>
        inline T_container& populateIndeces(T_container& c, T_ind min, T_ind max){
            static_assert(std::is_integral<T_ind>::value, "template argument must be an integral type");
            assert(c.size() < (max - min));
            std::set<size_t> indeces;
            while (indeces.size() < c.size()){
                indeces.insert(random_index(min, max));
            }
            std::copy(indeces.begin(), indeces.end(), c.begin());
            return c;
        }

		template<typename integer_t>
		class IntIterator;

		//Returns an random integer iterator with defined min and max values
		template<typename integer_t>
		IntIterator<integer_t> getIntegerIterator(integer_t min, integer_t max)
		{
			return IntIterator<integer_t>(engine_, min, max);
		}

		template<typename integer_t>
		class IntIterator
		{
		private:
			friend IntIterator<integer_t> Random::getIntegerIterator(integer_t min, integer_t max);

			std::uniform_int_distribution<integer_t> dist_;
			std::default_random_engine engine_;

			integer_t current_;
			IntIterator(std::default_random_engine& engine, integer_t min, integer_t max) : dist_(min, max), engine_(engine), current_(dist_(engine)) {}

		public:
			void setDistribution(std::uniform_int_distribution<integer_t> dist)
			{
				dist_ = dist;
			}

			std::uniform_int_distribution<integer_t> getDistribution()
			{
				return dist_;
			}

			integer_t operator ()() const
			{
				return current_;
			}

			operator integer_t() const
			{
				return current_;
			}

			integer_t current()
			{
				return current_;
			}

			integer_t next()
			{
				return (current_ = dist_(engine_));
			}

			integer_t operator ++()
			{
				current_ = dist_(engine_);
				return current_;
			}

			integer_t operator ++(integer_t)
			{
				const integer_t ret = current_;
				current_ = dist_(engine_);
				return ret;
			}
		};


		template<typename real_t>
		class RealIterator;

		//Returns an random real number iterator with defined min and max values
		template<typename real_t>
		RealIterator<real_t> getRealIterator(real_t min, real_t max)
		{
			return RealIterator<real_t>(engine_, min, max);
		}

		template<typename real_t>
		class RealIterator
		{
		private:
			friend RealIterator<real_t> Random::getRealIterator(real_t min, real_t max);

			std::uniform_real_distribution<real_t> dist_;
			std::default_random_engine engine_;

			real_t current_;
			RealIterator(std::default_random_engine& engine, real_t min, real_t max) : dist_(min, max), engine_(engine), current_(dist_(engine)) {}

		public:
			void setDistribution(std::uniform_real_distribution<real_t> dist)
			{
				dist_ = dist;
			}

			std::uniform_real_distribution<real_t> getDistribution()
			{
				return dist_;
			}

			real_t operator ()() const
			{
				return current_;
			}

			operator real_t() const
			{
				return current_;
			}

			real_t current()
			{
				return current_;
			}

			real_t next()
			{
				return (current_ = dist_(engine_));
			}

			real_t operator ++()
			{
				current_ = dist_(engine_);
				return current_;
			}

			real_t operator ++(int)
			{
				const real_t ret = current_;
				current_ = dist_(engine_);
				return ret;
			}
		};


		template<typename num_t, typename dist_t>
		class DistributionIterator;

		//Returns an random iterator defined by given distribution
		template<typename num_t, typename dist_t>
		DistributionIterator<num_t, dist_t> getDistributionIterator(dist_t dist)
		{
			return DistributionIterator<num_t, dist_t>(engine_, dist);
		}

		template<typename num_t, typename dist_t>
		class DistributionIterator
		{
		private:
			friend DistributionIterator<num_t, dist_t> Random::getDistributionIterator(dist_t dist);

			dist_t dist_;
			std::default_random_engine engine_;

			num_t current_;

			DistributionIterator(std::default_random_engine& engine, dist_t dist) : dist_(dist), engine_(engine), current_(dist_(engine)) {}

		public:
			void setDistribution(dist_t dist)
			{
				dist_ = dist;
			}

			dist_t getDistribution()
			{
				return dist_;
			}

			num_t operator ()() const
			{
				return current_;
			}

			operator num_t() const
			{
				return current_;
			}

			num_t current()
			{
				return current_;
			}

			num_t next()
			{
				return (current_ = dist_(engine_));
			}

			num_t operator ++()
			{
				current_ = dist_(engine_);
				return current_;
			}

			num_t operator ++(int)
			{
				const num_t ret = current_;
				current_ = dist_(engine_);
				return ret;
			}
		};
	};

	namespace rng
	{
		//Global acces random number generator of type random
		//For ease of use it is recommended to use a reference like this:
		//	auto& rng = kki::random::global_rng;
		static Random global;
		//Thread local random number generator of type random
		//For ease of use it is recommended to use a reference like this:
		//	auto& tl = kki::random::tl_rng;
		thread_local static Random tl;
	}
}