#pragma once
#include <iostream>
#include <vector>
#include <GFramework/GVariant/GVariant.h>

namespace GFramework
{
	template <typename T> struct is_shared_pointer {
	private:
		template <typename C> struct _is_shared_pointer { static constexpr bool value = false; };
		template <typename C> struct _is_shared_pointer< std::shared_ptr<C> > { static constexpr bool value = true; };
	public:
		static constexpr bool value = _is_shared_pointer< typename std::decay<T>::type>::value;
	};

	template <typename T> struct is_unique_pointer {
	private:
		template <typename C> struct _is_unique_pointer { static constexpr bool value = false; };
		template <typename C> struct _is_unique_pointer< std::unique_ptr<C> > { static constexpr bool value = true; };
	public:
		static constexpr bool value = _is_unique_pointer< typename std::decay<T>::type>::value;
	};

	template <typename T> struct is_weak_pointer {
	private:
		template <typename C> struct _is_weak_pointer { static constexpr bool value = false; };
		template <typename C> struct _is_weak_pointer< std::weak_ptr<C> > { static constexpr bool value = true; };
	public:
		static constexpr bool value = _is_weak_pointer< typename std::decay<T>::type>::value;
	};

	template <typename T> struct is_smart_pointer {
		static constexpr bool value = (is_weak_pointer<T>::value || is_unique_pointer<T>::value || is_shared_pointer<T>::value);
	};

	typedef struct {
		void* object;
	} Glua_abstract_ptr;

	template<typename T>
	struct generic_smart_pointer {
		static void lua_push(lua_State* L, T v)
		{
			if (v)
			{
				lua_pushlightuserdata(L, v.get());
			}
			else
			{
				lua_pushnil(L);
			}
		}
	};

	template<typename T>
	struct object_smart_pointer {
		static void lua_push(lua_State* L, T v)
		{
			if (v)
			{
				const char* metaclass_name = v->getMetaclass()->getName().c_str();
				size_t nbytes = sizeof(Glua_abstract_ptr);
				Glua_abstract_ptr* userdata = (Glua_abstract_ptr*)lua_newuserdata(L, nbytes);
				userdata->object = v.get();

				luaL_getmetatable(L, metaclass_name);
				lua_setmetatable(L, -2);
			}
			else
			{
				lua_pushnil(L);
			}
		}
	};

	template<typename T>
	struct smart_pointer {
		static void lua_push(lua_State* L, T v)
		{
			typedef typename std::conditional<std::is_base_of<GObject, typename std::decay<decltype(*v)>::type>::value, object_smart_pointer<T>, generic_smart_pointer<T> >::type selected_type;
			selected_type::lua_push(L, v);
		}
	};

	template<typename T>
	struct generic_data {
		static void lua_push(lua_State* L, T v)
		{
			lua_pushlightuserdata(L, &v);
		}
	};

	template<typename T>
	static void lua_pusher(lua_State* L, T v)
	{
		typedef typename std::conditional<is_smart_pointer<T>::value,smart_pointer<T>,generic_data<T>>::type selected_type;
		selected_type::lua_push(L, v);
	}

	template<>
	void lua_pusher(lua_State* L, unsigned int v)
	{
		lua_pushinteger(L, v);
	}

	template<>
	void lua_pusher(lua_State* L, int v)
	{
		lua_pushinteger(L, v);
	}

	template<>
	void lua_pusher(lua_State* L, const std::string v)
	{
		lua_pushstring(L, v.c_str());
	}

	template<>
	void lua_pusher(lua_State* L, glm::vec3 v)
	{

	}

	template<typename T>
	void Glua_pusher(lua_State* L, GVariant& rv)
	{
		//std::cout << typeid(T).name() << std::endl;
		lua_pusher(L, boost::any_cast<T>(rv));
	}

	template<>
	void GFRAMEWORK_API Glua_pusher<void>(lua_State* L, GVariant& rv);

	//TODO: Try to avoid the offset parameter and handle member function and non member fumction arguments efficiently
	template<typename T>
	int Glua_puller(lua_State* L, std::vector<GVariant>& args, int offset=2)
	{
		std::cout << "No Glua_puller function implemented to read data of type " << typeid(T).name() << " from the lua scripts." << std::endl;
		return 1;
	}

	template<>
	int GFRAMEWORK_API Glua_puller<unsigned int>(lua_State* L, std::vector<GVariant>& args, int offset);

	template<>
	int GFRAMEWORK_API Glua_puller<int>(lua_State* L, std::vector<GVariant>& args, int offset);

	template<>
	int GFRAMEWORK_API Glua_puller<float>(lua_State* L, std::vector<GVariant>& args, int offset);

	template<>
	int GFRAMEWORK_API Glua_puller<std::string>(lua_State* L, std::vector<GVariant>& args, int offset);


}
