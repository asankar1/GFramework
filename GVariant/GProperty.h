#pragma once
#include <typeinfo>
#include <typeindex>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <glm/gtc/type_ptr.hpp>
#include <GFramework/GVariant/GTypes.h>
//#include <GSerializer.h>
#include <GFramework/GVariant/GVariant.h>

namespace GFramework
{
	class GObject;
	class GPropertyInterface;
	typedef std::shared_ptr<GPropertyInterface> GPropertyInterfaceSharedPtr;
	typedef std::unique_ptr<GPropertyInterface> GPropertyInterfaceUniquePtr;

	void register_lua_script_functions(lua_State* L, std::vector<luaL_Reg>& GPropertiesList);

	template <typename T>
	struct GFRAMEWORK_API GPropertyUtility
	{
		//static_assert(false, "This template must be specialized for each property type, this will be used by GReflection");
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(T value);
	};

#if 0
	template <typename T>
	class GFRAMEWORK_API GPropertyConverter
	{
	public:
		//static_assert(false, "This template must be specialized for each property type, this will be used by GReflection");
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(T v);
		static GPropertyInterfaceUniquePtr convertDataToGProperty(T t);
		static T parseTextToData(std::istream& is);
	};
#endif

	class GFRAMEWORK_API GPropertyInterface
	{
	public:
		/*template<typename T>
		static public GPropertyInterfaceUniquePtr ToProperty(T value)
		{
			return GPropertyConverter<T>::convertDataToGProperty(value);
		}*/
		virtual void set(GVariant& _value) = 0;
		virtual GVariant get() const = 0;
		virtual std::ostream& writeBinaryValue(std::ostream& os) const = 0;
		virtual std::istream& readBinaryValue(std::istream& is) = 0;
		virtual std::ostream& writeASCIIValue(std::ostream& os) const = 0;
		virtual std::istream& readASCIIValue(std::istream& is) = 0;
		virtual bool isGObjectPointer() const {
			return false;
		}

		virtual std::type_index getPointedGObjectTypeIndex() {
			assert(false && "getPointedGObjectTypeIndex cannot be called on a property that is not GPointerProperty<>");
			return typeid(void);
		}
	protected:
		GPropertyInterface() {}
	};

	template <typename T>
	class GFRAMEWORK_API GArithmeticProperty : public GPropertyInterface
	{
		static_assert(std::is_arithmetic<T>::value, "Template argument T to GArithmeticProperty must be an arithmetic type!");
	public:
		GArithmeticProperty(T v = std::numeric_limits<T>::min());

		virtual void set(GVariant& _value);

		void setValue(T _value);

		virtual GVariant get() const;

		const T& getValue() const;

		std::ostream& writeASCIIValue(std::ostream& os) const;

		std::istream& readASCIIValue(std::istream& is);

		std::ostream& writeBinaryValue(std::ostream& os) const;

		std::istream& readBinaryValue(std::istream& is);

	private:
		T value;
	};

	class GFRAMEWORK_API GStringProperty : public GPropertyInterface
	{
	public:
		GStringProperty(const std::string v = "");

		virtual void set(GVariant& _value);

		void setValue(const std::string& _value);

		virtual GVariant get() const;

		const std::string& getValue() const;

		std::ostream& writeASCIIValue(std::ostream& os) const;

		std::istream& readASCIIValue(std::istream& is);

		std::ostream& writeBinaryValue(std::ostream& os) const;

		std::istream& readBinaryValue(std::istream& is);

	private:
		std::string value;
	};

	template <typename T>
	class GFRAMEWORK_API GGlmProperty : public GPropertyInterface
	{
	public:
		GGlmProperty(T v = T());

		virtual void set(GVariant& _value);

		void setValue(T _value);

		virtual GVariant get() const;

		const T& getValue() const;

		std::ostream& writeASCIIValue(std::ostream& os) const;

		std::istream& readASCIIValue(std::istream& is);

		std::ostream& writeBinaryValue(std::ostream& os) const;

		std::istream& readBinaryValue(std::istream& is);

	private:
		T value;
	};

	class  GPointerPropertyInterface : public GPropertyInterface
	{
	public:
		virtual unsigned int getObjectId() const = 0;
		virtual void subjectDeleted() = 0;
		GPointerPropertyInterface(const GPointerPropertyInterface&) = delete;
		GPointerPropertyInterface& operator=(const GPointerPropertyInterface&) = delete;
		virtual GObjectSharedPtr getGObjectPointer() const = 0;
		virtual void setGObjectPointer(GObjectSharedPtr) = 0;

	protected:
		GPointerPropertyInterface() {}
	};

	template<typename T>
	class  GPointerProperty : public GPointerPropertyInterface
	{
	public:
		GPointerProperty(std::shared_ptr<T> v = std::shared_ptr<T>(nullptr)) : value(v) {
			static_assert(std::is_base_of<GObject, T>::value, "Template argument T to GPointerProperty must be derived from Object class directly or indirectly!");
		}

		virtual unsigned int getObjectId() const
		{
			static_assert(std::is_base_of<GObject, T>::value, "Template argument T to GPointerProperty must be derived from Object class directly or indirectly!");
			return objectId;
		}

		virtual ~GPointerProperty() {
			if (value != nullptr) {
				value->unSubscribeDeletionNotification(this);
			}
		}

		virtual bool isGObjectPointer() const {
			return true;
		}

		virtual std::type_index getPointedGObjectTypeIndex() {
			return typeid(std::shared_ptr<T>);
		}

		virtual void set(GVariant& _value) {
			setValue(GVariant::cast<std::shared_ptr<T>>(_value));
		}

		void setValue(std::shared_ptr<T> _value) {
			if (value != nullptr) {
				value->unSubscribeDeletionNotification(this);
			}
			value = _value;
			objectId = value->getObjectId();
			if (value != nullptr) {
				value->subscribeDeletionNotification(this);
			}
		}
		
		virtual void setGObjectPointer(GObjectSharedPtr v)
		{
			value = std::dynamic_pointer_cast<T>(v);
			objectId = value->getObjectId();
		}

		virtual GObjectSharedPtr getGObjectPointer() const
		{
			return value;
		}

		virtual GVariant get() const {
			return GVariant::create < std::shared_ptr<T> >(value);
		}

		std::shared_ptr<T> getValue() const {
			return value;
		}

		virtual void subjectDeleted() {
			value = nullptr;
		}

		virtual std::ostream& writeBinaryValue(std::ostream& os) const {
			static_assert(std::is_base_of<GObject, T>::value, "Template argument T to GPointerProperty must be derived from Object class directly or indirectly!");
			if (value)
			{
				os << value->getObjectId();
			}
			else
			{
				os << (uint32)0;
			}
			return os;
		}

		virtual std::istream& readBinaryValue(std::istream& is) {
			is >> objectId;
			if (objectId)
			{
				//TODO: Fix: GDeserializer::addReferenceSeeker(pointer_obj_id, value);
			}
			return is;
		}

		virtual std::ostream& writeASCIIValue(std::ostream& os) const {
			static_assert(std::is_base_of<GObject, T>::value, "Template argument T to GPointerProperty must be derived from Object class directly or indirectly!");
			if (value)
			{
				os << value->getObjectId();
			}
			else
			{
				os << (uint32)0;
			}
			return os;
		}
		virtual std::istream& readASCIIValue(std::istream& is) {
			is >> objectId;
			if (objectId)
			{
				//TODO: Fix: GDeserializer::addReferenceSeeker(pointer_obj_id, value);
			}
			return is;
		}

	private:
		std::shared_ptr<T> value;
		unsigned int objectId=0;
	};

#if 0
	template <>
	class GFRAMEWORK_API GPropertyConverter<glm::vec2>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(glm::vec2 value)
		{
			return std::make_unique< GGlmProperty<glm::vec2> >(value);
		}

		static auto parseTextToData(std::istream& is)
		{
			glm::vec2 value;
			const unsigned int size = sizeof(decltype(value)) / sizeof(decltype(value)::value_type);
			float* pointer = glm::value_ptr(value);
			for (unsigned int i = 0; i < size; i++)
			{
				is >> std::hex >> *((int32*)pointer + i);
			}
			return value;
		}
	};

	template <>
	class GFRAMEWORK_API GPropertyConverter<glm::vec3>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(glm::vec3 value)
		{
			return std::make_unique< GGlmProperty<glm::vec3> >(value);
		}

		static auto parseTextToData(std::istream& is)
		{
			glm::vec3 value;
			const unsigned int size = sizeof(decltype(value)) / sizeof(decltype(value)::value_type);
			float* pointer = glm::value_ptr(value);
			for (unsigned int i = 0; i < size; i++)
			{
				is >> std::hex >> *((int32*)pointer + i);
			}
			return value;
		}
	};

	template <>
	class GFRAMEWORK_API GPropertyConverter<glm::vec4>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(glm::vec4 value)
		{
			return std::make_unique< GGlmProperty<glm::vec4> >(value);
		}

		static auto parseTextToData(std::istream& is)
		{
			glm::vec4 value;
			const unsigned int size = sizeof(decltype(value)) / sizeof(decltype(value)::value_type);
			float* pointer = glm::value_ptr(value);
			for (unsigned int i = 0; i < size; i++)
			{
				is >> std::hex >> *((int32*)pointer + i);
			}
			return value;
		}
	};

	template <>
	class GFRAMEWORK_API GPropertyConverter<glm::mat2>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(glm::mat2 value)
		{
			return std::make_unique< GGlmProperty<glm::mat2> >(value);
		}

		static auto parseTextToData(std::istream& is)
		{
			glm::mat2 value;
			const unsigned int size = sizeof(decltype(value)) / sizeof(decltype(value)::value_type);
			float* pointer = glm::value_ptr(value);
			for (unsigned int i = 0; i < size; i++)
			{
				is >> std::hex >> *((int32*)pointer + i);
			}
			return value;
		}
	};

	template <>
	class GFRAMEWORK_API GPropertyConverter<glm::mat3>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(glm::mat3 value)
		{
			return std::make_unique< GGlmProperty<glm::mat3> >(value);
		}

		static auto parseTextToData(std::istream& is)
		{
			glm::mat3 value;
			const unsigned int size = sizeof(decltype(value)) / sizeof(decltype(value)::value_type);
			float* pointer = glm::value_ptr(value);
			for (unsigned int i = 0; i < size; i++)
			{
				is >> std::hex >> *((int32*)pointer + i);
			}
			return value;
		}
	};

	template <>
	class GFRAMEWORK_API GPropertyConverter<glm::mat4>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(glm::mat4 value)
		{
			return std::make_unique< GGlmProperty<glm::mat4> >(value);
		}

		static auto parseTextToData(std::istream& is)
		{
			glm::mat4 value;
			const unsigned int size = sizeof(decltype(value)) / sizeof(decltype(value)::value_type);
			float* pointer = glm::value_ptr(value);
			for (unsigned int i = 0; i < size; i++)
			{
				is >> std::hex >> *((int32*)pointer + i);
			}
			return value;
		}
	};

	template <>
	class GFRAMEWORK_API GPropertyConverter<bool>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(bool value)
		{
			return std::make_unique< GArithmeticProperty<bool> >(value);
		}
	};

	template <>
	class GFRAMEWORK_API GPropertyConverter<int8>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(int8 value)
		{
			return std::make_unique< GArithmeticProperty<int8> >(value);
		}
	};

	template <>
	class GFRAMEWORK_API GPropertyConverter<uint8>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(uint8 value)
		{
			return std::make_unique< GArithmeticProperty<uint8> >(value);
		}
	};

	template <>
	class GFRAMEWORK_API GPropertyConverter<int16>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(int16 value)
		{
			return std::make_unique< GArithmeticProperty<int16> >(value);
		}
	};

	template <>
	class GFRAMEWORK_API GPropertyConverter<uint16>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(uint16 value)
		{
			return std::make_unique< GArithmeticProperty<uint16> >(value);
		}
	};

	template <>
	class GFRAMEWORK_API GPropertyConverter<int32>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(int32 value)
		{
			return std::make_unique< GArithmeticProperty<int32> >(value);
		}
	};

	template <>
	class GFRAMEWORK_API GPropertyConverter<uint32>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(uint32 value)
		{
			return std::make_unique< GArithmeticProperty<uint32> >(value);
		}
	};

	template <>
	class GFRAMEWORK_API GPropertyConverter<int64>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(int64 value)
		{
			return std::make_unique< GArithmeticProperty<int64> >(value);
		}
	};

	template <>
	class GFRAMEWORK_API GPropertyConverter<uint64>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(uint64 value)
		{
			return std::make_unique< GArithmeticProperty<uint64> >(value);
		}
	};

	template <>
	class GFRAMEWORK_API GPropertyConverter<float>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(float value)
		{
			return std::make_unique< GArithmeticProperty<float> >(value);
		}
	};

	template <>
	class GFRAMEWORK_API GPropertyConverter<double>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(double value)
		{
			return std::make_unique< GArithmeticProperty<double> >(value);
		}
	};

	template <>
	class GFRAMEWORK_API GPropertyConverter <std::string>
	{
	public:
		static GPropertyInterfaceUniquePtr convertDataToGProperty(std::string value)
		{
			return std::make_unique< GStringProperty >(value);
		}
	};
#endif
	/*template<typename T>
	class  GProperty : public GPropertyInterface
	{
	public:

	virtual void set(GVariant& _value) {
	value = boost::any_cast<T>(_value);
	}

	void setValue(T _value) {
	value = _value;
	}

	virtual GVariant get() {
	return value;
	}

	const T& getValue() const {
	return value;
	}

	virtual std::ostream& writeBinaryValue(std::ostream& os) const { return os; }
	virtual std::istream& readBinaryValue(std::istream& is) { return is; }
	virtual std::ostream& writeASCIIValue(std::ostream& os) const { return os; }
	virtual std::istream& readASCIIValue(std::istream& is) { return is; }

	private:
	T value;
	};*/

	typedef GGlmProperty<glm::vec2> GVec2Property;
	typedef GGlmProperty<glm::vec3> GVec3Property;
	typedef GGlmProperty<glm::vec4> GVec4Property;
	typedef GGlmProperty<glm::mat2> GMat2Property;
	typedef GGlmProperty<glm::mat3> GMat3Property;
	typedef GGlmProperty<glm::mat4> GMat4Property;
	typedef GArithmeticProperty<bool> GBoolProperty;
	typedef GArithmeticProperty<char> GCharProperty;
	typedef GArithmeticProperty<int8> GInt8Property;
	typedef GArithmeticProperty<uint8> GUint8Property;
	typedef GArithmeticProperty<int16> GInt16troperty;
	typedef GArithmeticProperty<uint16> GUint16Property;
	typedef GArithmeticProperty<int32> GInt32Property;
	typedef GArithmeticProperty<uint32> GUint32Property;
	typedef GArithmeticProperty<int64> GInt64Property;
	typedef GArithmeticProperty<uint64> GUint64Property;
	typedef GArithmeticProperty<float> GFloatProperty;
	typedef GArithmeticProperty<double> GDoubleProperty;
	typedef GPointerProperty<GObject> GObjectPointerProperty;
	//typedef GPointerProperty<Node> GNodePointerProperty;

	template <>
	struct GFRAMEWORK_API GPropertyUtility<bool>
	{
		typedef bool dataType;
		typedef GArithmeticProperty<dataType> GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};

	template <>
	struct GFRAMEWORK_API GPropertyUtility<int8>
	{
		typedef int8 dataType;
		typedef GArithmeticProperty<dataType> GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};

	template <>
	struct GFRAMEWORK_API GPropertyUtility<uint8>
	{
		typedef uint8 dataType;
		typedef GArithmeticProperty<dataType> GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};

	template <>
	struct GFRAMEWORK_API GPropertyUtility<int16>
	{
		typedef int16 dataType;
		typedef GArithmeticProperty<dataType> GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};

	template <>
	struct GFRAMEWORK_API GPropertyUtility<uint16>
	{
		typedef uint16 dataType;
		typedef GArithmeticProperty<dataType> GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};

	template <>
	struct GFRAMEWORK_API GPropertyUtility<int32>
	{
		typedef int32 dataType;
		typedef GArithmeticProperty<dataType> GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};

	template <>
	struct GFRAMEWORK_API GPropertyUtility<uint32>
	{
		typedef uint32 dataType;
		typedef GArithmeticProperty<dataType> GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};

	template <>
	struct GFRAMEWORK_API GPropertyUtility<int64>
	{
		typedef int64 dataType;
		typedef GArithmeticProperty<dataType> GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};

	template <>
	struct GFRAMEWORK_API GPropertyUtility<uint64>
	{
		typedef uint64 dataType;
		typedef GArithmeticProperty<dataType> GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};

	template <>
	struct GFRAMEWORK_API GPropertyUtility<float>
	{
		typedef float dataType;
		typedef GArithmeticProperty<dataType> GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};

	template <>
	struct GFRAMEWORK_API GPropertyUtility<double>
	{
		typedef double dataType;
		typedef GArithmeticProperty<dataType> GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};


	template <>
	struct GFRAMEWORK_API GPropertyUtility<glm::vec2>
	{
		typedef glm::vec2 dataType;
		typedef GGlmProperty<dataType> GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};

	template <>
	struct GFRAMEWORK_API GPropertyUtility<glm::vec3>
	{
		typedef glm::vec3 dataType;
		typedef GGlmProperty<dataType> GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};

	template <>
	struct GFRAMEWORK_API GPropertyUtility<glm::vec4>
	{
		typedef glm::vec4 dataType;
		typedef GGlmProperty<dataType> GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};

	template <>
	struct GFRAMEWORK_API GPropertyUtility<glm::mat2>
	{
		typedef glm::mat2 dataType;
		typedef GGlmProperty<dataType> GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};

	template <>
	struct GFRAMEWORK_API GPropertyUtility<glm::mat3>
	{
		typedef glm::mat3 dataType;
		typedef GGlmProperty<dataType> GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};

	template <>
	struct GFRAMEWORK_API GPropertyUtility<glm::mat4>
	{
		typedef glm::mat4 dataType;
		typedef GGlmProperty<dataType> GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};

	template <>
	struct GFRAMEWORK_API GPropertyUtility<std::string>
	{
		typedef std::string dataType;
		typedef GStringProperty GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};

	template <>
	struct GFRAMEWORK_API GPropertyUtility<GObjectSharedPtr>
	{
		typedef GObjectSharedPtr dataType;
		typedef GObjectPointerProperty GPropertyType;
		static GPropertyInterfaceSharedPtr create();
		static GPropertyInterfaceSharedPtr create(dataType value);
	};
}
