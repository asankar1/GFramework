#pragma once
#include <map>
#include <mutex>
#include <vector>
#include <fstream>
#include <GReflection.h>

#ifdef VARIANT_DYNAMIC_LIBRARY
#ifdef DLL_EXPORT
#define LIBRARY_API __declspec( dllexport )
#else
#define LIBRARY_API __declspec( dllimport )
#endif
#else
#define LIBRARY_API
#endif

namespace GFramework
{
	class Object;

	class LIBRARY_API GSerializer
	{
	public:
		virtual ~GSerializer();
		virtual bool open(const char* filename) = 0;
		virtual void close();
		virtual GSerializer& operator<<(Object& _obj) = 0;
		virtual GSerializer& operator<<(Object* _obj) = 0;
		virtual bool writeMetaProperty(const Object* _obj, GMetaproperty* property) = 0;

	protected:
		static std::mutex reference_seeker_mutex;
		static std::mutex reference_provider_mutex;
		static std::map<unsigned int, std::vector<Object*>> reference_seekers;
		static std::map<unsigned int, std::vector<Object*>> reference_providers;
		std::ofstream stream;
	};

#if 1
	class LIBRARY_API GBinarySerializer : public GSerializer
	{
	public:
		virtual ~GBinarySerializer();
		virtual bool open(const char* filename) override;
		virtual bool writeMetaProperty(const Object* _obj, GMetaproperty* property) override;
		virtual GSerializer& operator<<(Object& _obj) override;
		virtual GSerializer& operator<<(Object* _obj) override;
};
#endif
	class LIBRARY_API GTextSerializer : public GSerializer
	{
	public:
		virtual ~GTextSerializer();
		virtual bool open(const char* filename) override;
		virtual bool writeMetaProperty(const Object* _obj, GMetaproperty* property) override;
		virtual GSerializer& operator<<(Object& _obj) override;
		virtual GSerializer& operator<<(Object* _obj) override;
	};

	class LIBRARY_API GDeserializer
	{
	public:
		virtual ~GDeserializer();
		virtual bool open(const char* filename) = 0;
		virtual void close();
		virtual GDeserializer& operator>>(Object** _obj) = 0;
		virtual bool readMetaProperty(Object* _obj, GMetaproperty* property) = 0;
		static void addReferenceSeeker(unsigned int _object_id, NodeSharedPtr* _seeking_object);
		static void addReferenceProviders(unsigned int _object_id, Object* _providing_object);
		void resolveDependencies();
	protected:
		static std::mutex reference_seeker_mutex;
		static std::mutex reference_provider_mutex;
		static std::map<unsigned int, std::vector<NodeSharedPtr*>> reference_seekers;
		static std::map<unsigned int, Object*> reference_providers;
		std::ifstream stream;
	};

	class LIBRARY_API GBinaryDeSerializer : public GDeserializer
	{
	public:
		virtual ~GBinaryDeSerializer();
		virtual bool open(const char* filename) override;
		virtual GDeserializer& operator >> (Object** _obj) override;
		virtual bool readMetaProperty(Object* _obj, GMetaproperty* property) override;
	};

	class LIBRARY_API GTextDeSerializer : public GDeserializer
	{
	public:
		virtual ~GTextDeSerializer();
		virtual bool open(const char* filename) override;
		virtual GDeserializer& operator >> (Object** _obj) override;
		virtual bool readMetaProperty(Object* _obj, GMetaproperty* property) override;
	};
}