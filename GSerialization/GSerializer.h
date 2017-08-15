#pragma once
#include <map>
#include <mutex>
#include <vector>
#include <fstream>

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
		GSerializer(std::ofstream& _stream);
		~GSerializer();
		virtual GSerializer& operator<<(Object& _obj) = 0;
		virtual GSerializer& operator<<(Object* _obj) = 0;
		static void addReferenceSeeker(unsigned int _object_id, Object* _seeking_object);
		static void addReferenceProviders(unsigned int _object_id, Object* _seeking_object);

	protected:
		static std::mutex reference_seeker_mutex;
		static std::mutex reference_provider_mutex;
		static std::map<unsigned int, std::vector<Object*>> reference_seekers;
		static std::map<unsigned int, std::vector<Object*>> reference_providers;
		std::ofstream& stream;
	};

	class LIBRARY_API GBinarySerializer : public GSerializer
	{
	public:
		GBinarySerializer(std::ofstream& _stream) : GSerializer(_stream) {

		}
		virtual GSerializer& operator<<(Object& _obj);
		virtual GSerializer& operator<<(Object* _obj);
	};

	class LIBRARY_API GTextSerializer : public GSerializer
	{
	public:
		GTextSerializer(std::ofstream& _stream) : GSerializer(_stream) {

		}
		virtual GSerializer& operator<<(Object& _obj);
		virtual GSerializer& operator<<(Object* _obj);
	};

	class LIBRARY_API GDeserializer
	{
	public:
		GDeserializer(std::ifstream& _stream);
		~GDeserializer();
		virtual GDeserializer& operator>>(Object** _obj) = 0;
		static void addReferenceSeeker(unsigned int _object_id, NodeSharedPtr* _seeking_object);
		static void addReferenceProviders(unsigned int _object_id, Object* _providing_object);
		void resolveDependencies();
	protected:
		static std::mutex reference_seeker_mutex;
		static std::mutex reference_provider_mutex;
		static std::map<unsigned int, std::vector<NodeSharedPtr*>> reference_seekers;
		static std::map<unsigned int, Object*> reference_providers;
		std::ifstream& stream;
	};

	class LIBRARY_API GBinaryDeSerializer : public GDeserializer
	{
	public:
		GBinaryDeSerializer(std::ifstream& _stream) : GDeserializer(_stream) {

		}

		virtual GDeserializer& operator >> (Object** _obj);
	};

	class LIBRARY_API GTextDeSerializer : public GDeserializer
	{
	public:
		GTextDeSerializer(std::ifstream& _stream) : GDeserializer(_stream) {

		}

		virtual GDeserializer& operator >> (Object** _obj);
	};

}