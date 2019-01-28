#include <sstream>
#include <iomanip>
#include <GVariant/GProperty.h>
using namespace std;

namespace GFramework
{
	static int tostring(lua_State *L)
	{
		lua_getmetatable(L, 1);
		lua_pushstring(L, "__name");
		lua_rawget(L, 2);
		const char * name = luaL_checkstring(L, -1);

		{
			GVec2Property* userdata = (GVec2Property*)luaL_testudata(L, 1, "Vec2_type");
			if (userdata)
			{
				std::ostringstream s;
				s << "x: " << userdata->getValue().x << " y: " << userdata->getValue().y;
				lua_pushstring(L, s.str().c_str());
				return 1;
			}
		}

		{
			GVec3Property* userdata = (GVec3Property*)luaL_testudata(L, 1, "Vec3_type");
			if (userdata)
			{
				std::ostringstream s;
				s << "x: " << userdata->getValue().x << " y: " << userdata->getValue().y;
				lua_pushstring(L, s.str().c_str());
				return 1;
			}
		}

		lua_pushstring(L, "!!Error! Attempt to print unknown GVariant type.!!");
		return 1;
	}

	static int new_vec2_data(lua_State *L)
	{
		luaL_newmetatable(L, "Vec2_type");

		lua_pushstring(L, "__tostring");
		lua_pushcfunction(L, tostring);
		lua_settable(L, -3);

		size_t nbytes = sizeof(GVec2Property);
		GVec2Property* userdata = (GVec2Property*)lua_newuserdata(L, nbytes);
		userdata->setValue(glm::vec2(1,13));

		luaL_getmetatable(L, "Vec2_type");
		lua_setmetatable(L, -2);

		return 1;
	}

	void register_lua_script_functions(lua_State *L, vector<luaL_Reg>& GPropertiesList)
	{
		GPropertiesList.push_back({ "newVec2", new_vec2_data });
	}	

	template <typename T>
	GArithmeticProperty<T>::GArithmeticProperty(T v):value(v) {
	
	}

	template <typename T>
	void GArithmeticProperty<T>::set(GVariant& _value) {
		value = GVariant::cast<T>(_value);
	}

	template <typename T>
	void GArithmeticProperty<T>::setValue(T _value) {
		value = _value;
	}

	template <typename T>
	GVariant GArithmeticProperty<T>::get() const {
		return GVariant::create<T>(value);
	}

	template <typename T>
	const T& GArithmeticProperty<T>::getValue() const {
		return value;
	}

	template <typename T>
	std::ostream& GArithmeticProperty<T>::writeASCIIValue(std::ostream& os) const {
		os << value << " ";
		return os;
	}

	template <>
	std::ostream& GArithmeticProperty<float>::writeASCIIValue(std::ostream& os) const {
		os << std::hex << *((int32*)&value) << " ";
		return os;
	}

	template <>
	std::ostream& GArithmeticProperty<double>::writeASCIIValue(std::ostream& os) const {
		os << std::hex << *((int64*)&value) << " ";
		return os;
	}

	template <typename T>
	std::istream& GArithmeticProperty<T>::readASCIIValue(std::istream& is) {
		is >> value;
		return is;
	}

	template <>
	std::istream& GArithmeticProperty<float>::readASCIIValue(std::istream& is) {
		is >> std::hex >> *((int32*)&value);
		return is;
	}

	template <>
	std::istream& GArithmeticProperty<double>::readASCIIValue(std::istream& is) {
		is >> std::hex >> *((int64*)&value);
		return is;
	}

	template <typename T>
	std::ostream& GArithmeticProperty<T>::writeBinaryValue(std::ostream& os) const {
		os.write(reinterpret_cast<const char*>(&value), sizeof(T));
		return os;
	}

	template <typename T>
	std::istream& GArithmeticProperty<T>::readBinaryValue(std::istream& is) {
		is.read(reinterpret_cast<char*>(&value), sizeof(T));
		return is;
	}

	GStringProperty::GStringProperty() {

	}

	void GStringProperty::set(GVariant& _value) {
		value = GVariant::cast<std::string>(_value);
	}

	void GStringProperty::setValue(const std::string& _value) {
		value = _value;
	}

	GVariant GStringProperty::get() const {
		return GVariant::create<std::string>(value);
	}

	const std::string& GStringProperty::getValue() const {
		return value;
	}

	std::ostream& GStringProperty::writeASCIIValue(std::ostream& os) const {
		os << value.length() << " " << value << " ";
		return os;
	}

	std::istream& GStringProperty::readASCIIValue(std::istream& is) {
		size_t count;
		is >> count;
		std::string str;
		while (value.length() < count)
		{
			is >> str;
			value.append(str);
		}

		return is;
	}

	std::ostream& GStringProperty::writeBinaryValue(std::ostream& os) const {
		size_t size = value.length() + 1;
		os.write(reinterpret_cast<char*>(&size), sizeof(size_t));
		os.write(value.c_str(), size);
		return os;
	}

	std::istream& GStringProperty::readBinaryValue(std::istream& is) {
		size_t count;
		is.read(reinterpret_cast<char*>(&count), sizeof(size_t));
		char* str = new char[count];
		is.read(str, count);
		value = std::string(str);
		delete[] str;
		return is;
	}

	template <typename T>
	GGlmProperty<T>::GGlmProperty(T v):value(v) {}

	template <typename T>
	void GGlmProperty<T>::set(GVariant& _value) {
		value = GVariant::cast<T>(_value);
	}

	template <typename T>
	void GGlmProperty<T>::setValue(T _value) {
		value = _value;
	}

	template <typename T>
	GVariant GGlmProperty<T>::get() const {
		return GVariant::create<T>(value);
	}

	template <typename T>
	const T& GGlmProperty<T>::getValue() const {
		return value;
	}

	template <typename T>
	std::ostream& GGlmProperty<T>::writeASCIIValue(std::ostream& os) const {
		const unsigned int size = sizeof(typename T::type) / sizeof(typename T::value_type);
		const float* pointer = glm::value_ptr(value);
		for (unsigned int i = 0; i < size; i++)
		{
			//os << pointer[i] << " ";
			os << std::hex << *((int32*)pointer+i) << " ";
		}
		return os;
	}

	template <typename T>
	std::istream& GGlmProperty<T>::readASCIIValue(std::istream& is) {
		const unsigned int size = sizeof(typename T::type) / sizeof(typename T::value_type);
		float* pointer = glm::value_ptr(value);
		for (unsigned int i = 0; i < size; i++)
		{
			//is >> pointer[i];
			is >> std::hex >> *((int32*)pointer+i);
		}
		return is;
	}

	template <typename T>
	std::ostream& GGlmProperty<T>::writeBinaryValue(std::ostream& os) const {
		const unsigned int size = sizeof(typename T::type) / sizeof(typename T::value_type);
		const float* pointer = glm::value_ptr(value);
		os.write(reinterpret_cast<const char*>(pointer), size * sizeof(float));
		return os;
	}

	template <typename T>
	std::istream& GGlmProperty<T>::readBinaryValue(std::istream& is) {
		const unsigned int size = sizeof(typename T::type) / sizeof(typename T::value_type);
		float* pointer = glm::value_ptr(value);
		is.read(reinterpret_cast<char*>(pointer), size * sizeof(float));
		return is;
	}

	template class GArithmeticProperty<bool>;
	template class GArithmeticProperty<int8>;
	template class GArithmeticProperty<uint8>;
	template class GArithmeticProperty<int16>;
	template class GArithmeticProperty<uint16>;
	template class GArithmeticProperty<int32>;
	template class GArithmeticProperty<uint32>;
	template class GArithmeticProperty<int64>;
	template class GArithmeticProperty<uint64>;
	template class GArithmeticProperty<float>;
	template class GArithmeticProperty<double>;
	template class GGlmProperty<glm::vec2>;
	template class GGlmProperty<glm::vec3>;
	template class GGlmProperty<glm::vec4>;
	template class GGlmProperty<glm::mat2>;
	template class GGlmProperty<glm::mat3>;
	template class GGlmProperty<glm::mat4>;
}