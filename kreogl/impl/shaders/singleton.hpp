#pragma once

namespace kreogl {
	template<typename T>
	class singleton {
	public:
		static T & get_singleton() noexcept {
			static T singleton;
			return singleton;
		}
	};
}