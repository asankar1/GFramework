#pragma once
#include <map>
#include <mutex>
#include <vector>
#include <fstream>
//#include <GFramework/GReflection/GReflection.h>

namespace GFramework
{
	class GObject;
	class GMetaproperty;

	class GFRAMEWORK_API GSerializer
	{
	public:
		virtual ~GSerializer();
		virtual bool open(const char* filename) = 0;
		virtual void close();
		virtual GSerializer& operator<<(GObject& _obj) = 0;
		virtual GSerializer& operator<<(GObject* _obj) = 0;
		virtual bool writeMetaProperty(const GObject* _obj, GMetaproperty* property) = 0;

	protected:
		static std::mutex reference_seeker_mutex;
		static std::mutex reference_provider_mutex;
		static std::map<unsigned int, std::vector<GObject*>> reference_seekers;
		static std::map<unsigned int, std::vector<GObject*>> reference_providers;
		std::ofstream stream;
	};

#if 1
	class GFRAMEWORK_API GBinarySerializer : public GSerializer
	{
	public:
		virtual ~GBinarySerializer();
		virtual bool open(const char* filename) override;
		virtual bool writeMetaProperty(const GObject* _obj, GMetaproperty* property) override;
		virtual GSerializer& operator<<(GObject& _obj) override;
		virtual GSerializer& operator<<(GObject* _obj) override;
};
#endif
	class GFRAMEWORK_API GTextSerializer : public GSerializer
	{
	public:
		GTextSerializer();
		virtual ~GTextSerializer();
		virtual bool open(const char* filename) override;
		virtual bool writeMetaProperty(const GObject* _obj, GMetaproperty* property) override;
		virtual GSerializer& operator<<(GObject& _obj) override;
		virtual GSerializer& operator<<(GObject* _obj) override;
	protected:
		const char* objectDelimiter;
	};

	class GFRAMEWORK_API GDeserializer
	{
	public:
		virtual ~GDeserializer();
		virtual bool open(const char* filename) = 0;
		virtual void close();
		virtual GDeserializer& operator>>(GObject** _obj) = 0;
		virtual bool readMetaProperty(GObject* _obj, GMetaproperty* property) = 0;
		static void addReferenceSeeker(unsigned int _object_id, GObjectSharedPtr* _seeking_object);
		static void addReferenceProviders(unsigned int _object_id, GObject* _providing_object);
		void resolveDependencies();
		void setObject_id(GObject* _obj, uint32 id);
	protected:
		static std::mutex reference_seeker_mutex;
		static std::mutex reference_provider_mutex;
		static std::map<unsigned int, std::vector<GObject*>> reference_seekers;
		static std::map<unsigned int, GObject*> reference_providers;
		std::ifstream stream;
	};

	class GFRAMEWORK_API GBinaryDeSerializer : public GDeserializer
	{
	public:
		virtual ~GBinaryDeSerializer();
		virtual bool open(const char* filename) override;
		virtual GDeserializer& operator >> (GObject** _obj) override;
		virtual bool readMetaProperty(GObject* _obj, GMetaproperty* property) override;
	};

	class GFRAMEWORK_API GTextDeSerializer : public GDeserializer
	{
	public:
		GTextDeSerializer();
		virtual ~GTextDeSerializer();
		virtual bool open(const char* filename) override;
		virtual GDeserializer& operator >> (GObject** _obj) override;
		virtual bool readMetaProperty(GObject* _obj, GMetaproperty* property) override;
	protected:
		const char* objectDelimiter;
		virtual std::pair<std::string, std::string> parseProperty(std::string line);
	};
}