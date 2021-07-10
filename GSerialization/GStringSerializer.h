#pragma once
#include <GFramework/GSerialization/GSerializer.h>
namespace GFramework
{
	class GStringSerializer;
	class GStringDeserializer;

	namespace GStringSerializerWriter
	{
		template<typename T>
		GStringSerializer& write(GStringSerializer& stream, T& _obj)
		{
			static_assert (false, "You must specialize the write function template for GStringSerializer!");
			return stream;
		}

		template<>
		GFRAMEWORK_API GStringSerializer& write<int>(GStringSerializer& stream, int& _obj);

		/*template<>
		GFRAMEWORK_API GStringSerializer& write<GObject>(GStringSerializer& stream, GObject& _obj);*/
	}


	class GFRAMEWORK_API GStringSerializer : public GSerializer
	{
	public:
		GStringSerializer();
		virtual ~GStringSerializer();
		/*template<typename T>
		GStringSerializer& operator<<(T* value)
		{
			static_assert(!std::is_base_of<GObject, std::remove_cv<T>::type>::value, "Pointer to GObject or its derived class objects cannot be serialized!");
			NonGObjectPtrTypeHandler<T>::write(*this, value);
			return *this;
		}

		template<typename T>
		GStringSerializer& operator<<(T&& value)
		{
			//TODO: decide what to do if T is GObject rvalue reference and how it shlould be deserialized
			typedef std::conditional< std::is_base_of<GObject, std::remove_reference<std::remove_cv<T>::type>::type>::value, GObjectTypeHandler, NonGObjectTypeHandler<T> >::type Handler;
			Handler::write(*this, std::forward<T>(value));
			return *this;
		}*/
		virtual bool writeMetaProperty(const GObject* _obj, GMetaproperty* property);
	protected:
		virtual GSerializer& write(const GPropertyInterface& property);
		virtual GSerializer& write(const GObjectSharedPtr& _obj) override;
		/*struct GObjectTypeHandler
		{
			static void write(GStringSerializer& stream, GObject& obj)
			{
				stream << obj;
			}
		};

		template<typename T>
		struct NonGObjectTypeHandler
		{
			static void write(GStringSerializer& stream, T& value)
			{
				GStringSerializerWriter::write(stream, std::forward<T>(value));
			}
		};

		template<typename T>
		struct NonGObjectPtrTypeHandler
		{
			static void write(GStringSerializer& stream, T* value)
			{
				GStringSerializerWriter::write(stream, *value);
			}
		};*/
		const char* objectDelimiter;
	};

	namespace GStringDeserializerReader
	{
		template<typename T>
		GStringDeserializer& read(GStringDeserializer& stream, T& _obj)
		{
			static_assert (false, "You must specialize the write function template for GStringDeserializer!");
			return stream;
		}

		template<>
		GFRAMEWORK_API GStringDeserializer& read<int>(GStringDeserializer& stream, int& _obj);

		/*template<>
		GFRAMEWORK_API GStringSerializer& read<GObject>(GStringSerializer& stream, GObject& _obj);*/
	}


	class GFRAMEWORK_API GStringDeserializer : public GDeserializer
	{
	public:
		GStringDeserializer();
		virtual ~GStringDeserializer();
		virtual bool readMetaProperty(GObjectSharedPtr _obj, GMetaproperty* property);

		/*template<typename T>
		GStringDeserializer& operator>>(T* value)
		{
			static_assert(!std::is_base_of<GObject, std::remove_cv<T>::type>::value, "Pointer to GObject or its derived class objects cannot be deserialized, use GObject**");
			NonGObjectPtrTypeHandler<T>::read(*this, value);
			return *this;
		}

		template<typename T>
		GStringDeserializer& operator>>(T& value)
		{
			static_assert(!std::is_base_of<GObject, std::remove_cv<T>::type>::value, "Reference to GObject or its derived class objects cannot be deserialized, use GObject**");
			NonGObjectTypeHandler<T>::read(*this, value);
			return *this;
		}*/

		/*template<typename T>
		GStringDeserializer& operator>>(T& value)
		{
			GStringDeserializerReader::read(stream, value);
			return *this;
		}*/

		/*template<typename T>
		GDeserializer& operator>>(T** _obj)
		{
			static_assert(std::is_base_of<GObject, T>::value, "template type T must be derived from GObject!");
			*this >> (GObject**)(_obj);
			return *this;
		}*/

		//virtual GDeserializer& operator>>(GObject** _obj);

	protected:
		virtual GDeserializer& read(GPropertyInterface& property);

		virtual GDeserializer& read(GObjectSharedPtr* _obj);

		/*struct GObjectTypeHandler
		{
			static void read(GStringDeserializer& stream, GObject& obj)
			{
				stream >> obj;
			}
		};

		struct GObjectPtrTypeHandler
		{
			static void read(GStringDeserializer& stream, GObject* obj)
			{
				stream >> obj;
			}
		};*/

		template<typename T>
		struct NonGObjectTypeHandler
		{
			static void read(GStringDeserializer& stream, T& value)
			{
				GStringDeserializerReader::read(stream, value);
			}
		};

		template<typename T>
		struct NonGObjectPtrTypeHandler
		{
			static void read(GStringDeserializer& stream, T* value)
			{
				GStringDeserializerReader::read(stream, *value);
			}
		};
		const char* objectDelimiter;
		virtual std::pair<std::string, std::string> parseProperty(std::string line);
		virtual std::string findNextProperty(std::string line);
	};

}