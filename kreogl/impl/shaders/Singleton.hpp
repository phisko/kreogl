#pragma once

namespace kreogl {
	template<typename T>
	class Singleton {
	public:
		static T & getSingleton() noexcept {
			static T singleton;
			return singleton;
		}
	};
}