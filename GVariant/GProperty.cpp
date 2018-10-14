#include <property.h>
#include <sstream>
using namespace std;

namespace GFramework
{

	template class GScalarProperty<bool>;
	template class GScalarProperty<char>;
	template class GScalarProperty<unsigned char>;
	template class GScalarProperty<short>;
	template class GScalarProperty<unsigned short>;
	template class GScalarProperty<int>;
	template class GScalarProperty<unsigned int>;
	template class GScalarProperty<float>;
	template class GScalarProperty<double>;

	template class GGlmProperty<glm::vec2>;
	template class GGlmProperty<glm::vec3>;

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
	GScalarProperty<T>::GScalarProperty() {
	
	}

	template <typename T>
	void GScalarProperty<T>::set(GVariant& _value) {
		value = boost::any_cast<T>(_value);
	}

	template <typename T>
	void GScalarProperty<T>::setValue(T _value) {
		value = _value;
	}

	template <typename T>
	GVariant GScalarProperty<T>::get() {
		return value;
	}

	template <typename T>
	const T& GScalarProperty<T>::getValue() const {
		return value;
	}

	template <typename T>
	std::ostream& GScalarProperty<T>::writeASCIIValue(std::ostream& os) const {
		os << value << " ";
		return os;
	}

	template <typename T>
	std::istream& GScalarProperty<T>::readASCIIValue(std::istream& is) {
		is >> value;
		return is;
	}

	template <typename T>
	std::ostream& GScalarProperty<T>::writeBinaryValue(std::ostream& os) const {
		os.write(reinterpret_cast<const char*>(&value), sizeof(T));
		return os;
	}

	template <typename T>
	std::istream& GScalarProperty<T>::readBinaryValue(std::istream& is) {
		is.read(reinterpret_cast<char*>(&value), sizeof(T));
		return is;
	}

	GStringProperty::GStringProperty() {

	}

	void GStringProperty::set(GVariant& _value) {
		value = boost::any_cast<std::string>(_value);
	}

	void GStringProperty::setValue(const std::string& _value) {
		value = _value;
	}

	GVariant GStringProperty::get() {
		return value;
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
	GGlmProperty<T>::GGlmProperty() {}

	template <typename T>
	void GGlmProperty<T>::set(GVariant& _value) {
		value = boost::any_cast<T>(_value);
	}

	template <typename T>
	void GGlmProperty<T>::setValue(T _value) {
		value = _value;
	}

	template <typename T>
	GVariant GGlmProperty<T>::get() {
		return value;
	}

	template <typename T>
	const T& GGlmProperty<T>::getValue() const {
		return value;
	}

	template <typename T>
	std::ostream& GGlmProperty<T>::writeASCIIValue(std::ostream& os) const {
		unsigned int size = value.length();
		const float* pointer = glm::value_ptr(value);
		for (unsigned int i = 0; i < size; i++)
		{
			os << pointer[i] << " ";
		}
		return os;
	}

	template <typename T>
	std::istream& GGlmProperty<T>::readASCIIValue(std::istream& is) {
		unsigned int size = value.length();
		float* pointer = glm::value_ptr(value);
		for (unsigned int i = 0; i < size; i++)
		{
			is >> pointer[i];
		}
		return is;
	}

	template <typename T>
	std::ostream& GGlmProperty<T>::writeBinaryValue(std::ostream& os) const {
		unsigned int size = value.length();
		const float* pointer = glm::value_ptr(value);
		os.write(reinterpret_cast<const char*>(pointer), size * sizeof(float));
		return os;
	}

	template <typename T>
	std::istream& GGlmProperty<T>::readBinaryValue(std::istream& is) {
		unsigned int size = value.length();
		float* pointer = glm::value_ptr(value);
		is.read(reinterpret_cast<char*>(pointer), size * sizeof(float));
		return is;
	}
}