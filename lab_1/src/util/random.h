#pragma once
#include <random>
#include <chrono>

namespace kki
{
	class random
	{
		std::default_random_engine engine_;

	public:
		random(unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count()) : engine_(seed) {}

		std::default_random_engine& get_engine()
		{
			return engine_;
		}

		///Returns a random element from a sequence container
		///Prequisites, container must have at(int) and size() functions
		template<typename  container_t>
		auto& random_element(container_t& container)
		{
			return container.at(random_index(container.size()));
		}


		///Returns random boolean
		///Returns true one time out of n samples on average
		bool random_bool(size_t n = 2)
		{
			return std::uniform_int_distribution<size_t>(0, n-1)(engine_) == 0;
		}

		///Returns random integer of type integer_t in the range [0, max]
		template<typename integer_t = int>
		integer_t random_int()
		{
			return std::uniform_int_distribution<integer_t>()(engine_);
		}
		
		
		///Returns random integer of type integer_t in the range [0, max]
		template<typename integer_t = int>
		integer_t random_int(integer_t max)
		{
			return std::uniform_int_distribution<integer_t>(0, max)(engine_);
		}

		///Returns random integer of type integer_t in the range [min, max]
		template<typename integer_t = int>
		integer_t random_int(integer_t min, integer_t max)
		{
			return std::uniform_int_distribution<integer_t>(min, max)(engine_);
		}

		///Returns random integer of type integer_t in the range [min, max>
		template<typename integer_t = size_t>
		integer_t random_index(integer_t max)
		{
			return std::uniform_int_distribution<integer_t>(0, max - 1)(engine_);
		}

		///Returns random integer of type integer_t in the range [min, max>
		template<typename integer_t = size_t>
		integer_t random_index(integer_t min, integer_t max)
		{
			return std::uniform_int_distribution<integer_t>(min, max - 1)(engine_);
		}

		///Returns random double of type real_t in the range [0, 1]
		template<typename real_t = double>
		real_t random_real()
		{
			return std::uniform_real_distribution<real_t>()(engine_);
		}

		///Returns random double of type real_t in the range [0, max]
		template<typename real_t = double>
		real_t random_real(real_t max)
		{
			return std::uniform_real_distribution<real_t>(0, max)(engine_);
		}

		///Returns random double of type real_t in the range [min, max]
		template<typename real_t = double>
		real_t random_real(real_t min, real_t max)
		{
			return std::uniform_real_distribution<real_t>(min, max)(engine_);
		}

		///Returns random double of type real_t following the normal distribution with given mean and sigma
		template<typename real_t = double>
		real_t random_gaussian(real_t mean = 0, real_t sigma = 1)
		{
			return std::normal_distribution<real_t>(mean, sigma)(engine_);
		}

		///Returns random value of type value_t following the given distribution
		template<typename value_t, typename distribution>
		value_t random_value(distribution dist)
		{
			return dist(engine_);
		}

		///Returns random lowercase letter
		char random_lowercase()
		{
			return std::uniform_int_distribution<int>('a', 'z')(engine_);
		}

		///Returns random uppercase letter
		char random_uppercase()
		{
			return std::uniform_int_distribution<int>('A', 'Z')(engine_);
		}

		///Returns random char digit
		char random_digit()
		{
			return std::uniform_int_distribution<int>('0', '9')(engine_);
		}

		///Returns random digit, uppercase or lowercase letter
		char random_alnum()
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
		char_t random_char()
		{
			return std::uniform_int_distribution<int>(std::numeric_limits<char_t>::min(), std::numeric_limits<char_t>::max())(engine_);
		}
		
		///Returns random char
		char random_char(char min, char max)
		{
			return std::uniform_int_distribution<int>(min, max)(engine_);
		}


		template<typename integer_t>
		class int_iterator;

		//Returns an random integer iterator with defined min and max values
		template<typename integer_t>
		int_iterator<integer_t> get_integer_iterator(integer_t min, integer_t max)
		{
			return int_iterator<integer_t>(engine_, min, max);
		}

		template<typename integer_t>
		class int_iterator
		{
		private:
			friend int_iterator<integer_t> random::get_integer_iterator(integer_t min, integer_t max);

			std::uniform_int_distribution<integer_t> dist_;
			std::default_random_engine engine_;

			integer_t current_;
			int_iterator(std::default_random_engine& engine, integer_t min, integer_t max) : dist_(min, max), engine_(engine), current_(dist_(engine)) {}

		public:
			void set_distribution(std::uniform_int_distribution<integer_t> dist)
			{
				dist_ = dist;
			}

			std::uniform_int_distribution<integer_t> get_distribution()
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
		class real_iterator;

		//Returns an random real number iterator with defined min and max values
		template<typename real_t>
		real_iterator<real_t> get_real_iterator(real_t min, real_t max)
		{
			return real_iterator<real_t>(engine_, min, max);
		}

		template<typename real_t>
		class real_iterator
		{
		private:
			friend real_iterator<real_t> random::get_real_iterator(real_t min, real_t max);

			std::uniform_real_distribution<real_t> dist_;
			std::default_random_engine engine_;

			real_t current_;
			real_iterator(std::default_random_engine& engine, real_t min, real_t max) : dist_(min, max), engine_(engine), current_(dist_(engine)) {}

		public:
			void set_distribution(std::uniform_real_distribution<real_t> dist)
			{
				dist_ = dist;
			}

			std::uniform_real_distribution<real_t> get_distribution()
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
		class distribution_iterator;

		//Returns an random iterator defined by given distribution
		template<typename num_t, typename dist_t>
		distribution_iterator<num_t, dist_t> get_distribution_iterator(dist_t dist)
		{
			return distribution_iterator<num_t, dist_t>(engine_, dist);
		}

		template<typename num_t, typename dist_t>
		class distribution_iterator
		{
		private:
			friend distribution_iterator<num_t, dist_t> random::get_distribution_iterator(dist_t dist);

			dist_t dist_;
			std::default_random_engine engine_;

			num_t current_;

			distribution_iterator(std::default_random_engine& engine, dist_t dist) : dist_(dist), engine_(engine), current_(dist_(engine)) {}

		public:
			void set_distribution(dist_t dist)
			{
				dist_ = dist;
			}

			dist_t get_distribution()
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
        //For ease of use it is recommended to bind a reference like this:
        //	auto& rng = kki::random::global_rng;
		static random global{};
        //Thread local random number generator of type random
        //For ease of use it is recommended to bind a reference like this:
        //	auto& tl = kki::random::tl_rng;
        thread_local static random tl{};
	}

}