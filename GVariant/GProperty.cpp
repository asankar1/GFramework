#include <sstream>
#include <iomanip>
#include <GFramework/GVariant/GProperty.h>
#include <GFramework/GVariant/GObject.h>
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
		os << *((int32*)&value) << " ";
		return os;
	}

	template <>
	std::ostream& GArithmeticProperty<double>::writeASCIIValue(std::ostream& os) const {
		os << *((int64*)&value) << " ";
		return os;
	}

	template <typename T>
	std::istream& GArithmeticProperty<T>::readASCIIValue(std::istream& is) {
		is >> value;
		return is;
	}

	template <>
	std::istream& GArithmeticProperty<float>::readASCIIValue(std::istream& is) {
		is >> *((int32*)&value);
		return is;
	}

	template <>
	std::istream& GArithmeticProperty<double>::readASCIIValue(std::istream& is) {
		is >> *((int64*)&value);
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

	GStringProperty::GStringProperty(const std::string v) {
		value = v;
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
		value = std::string(count, '\0');
		is.read(&value[0], 1);//to remove the space between the count and first character
		is.read(&value[0], count);
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
			os << *((int32*)pointer+i) << " ";
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
			is >> *((int32*)pointer+i);
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
#if 0
	template <>
	class GFRAMEWORK_API GPropertyConverter<GObject>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(GObject* value)
		{
			return std::make_unique< GPointerProperty<GObject> >(value);
		}
	};
#endif

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
	//template class GPropertyConverter<GObject>;

	
	GPropertyInterfaceSharedPtr GPropertyUtility<glm::vec2>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<glm::vec2>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<glm::vec3>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<glm::vec3>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<glm::vec4>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<glm::vec4>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<glm::mat2>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<glm::mat2>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<glm::mat3>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<glm::mat3>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<glm::mat4>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<glm::mat4>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<bool>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<bool>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<int8>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<int8>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<uint8>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<uint8>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<int16>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<int16>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<uint16>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<uint16>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<int32>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<int32>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<uint32>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<uint32>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<int64>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<int64>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<uint64>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<uint64>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<float>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<float>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<double>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<double>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<std::string>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	GPropertyInterfaceSharedPtr GPropertyUtility<std::string>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}

	std::shared_ptr<GObjectPointerProperty>  GPropertyUtility<GObjectSharedPtr>::create()
	{
		return std::make_shared< GPropertyType >();
	}

	std::shared_ptr<GObjectPointerProperty> GPropertyUtility<GObjectSharedPtr>::create(dataType value)
	{
		return std::make_unique< GPropertyType >(value);
	}
}