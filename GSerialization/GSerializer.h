#pragma once
#include <map>
#include <mutex>
#include <vector>
#include <istream>
#include <ostream>
//#include <GFramework/GReflection/GReflection.h>

namespace GFramework
{
	class GObject;
	class GMetaproperty;

	/** \var typedef std::shared_ptr<std::istream> IStreamSharedPtr;
	*	\brief shared pointer type for the std::istream
	*/
	typedef std::shared_ptr<std::istream> IStreamSharedPtr;

	/** \var typedef std::shared_ptr<std::ostream> IStreamSharedPtr;
	*	\brief shared pointer type for the std::ostream
	*/
	typedef std::shared_ptr<std::ostream> OStreamSharedPtr;

	class GFRAMEWORK_API GSerializer
	{
	public:
		virtual ~GSerializer();
		virtual bool open(OStreamSharedPtr _stream);
		OStreamSharedPtr getStream();
		virtual void close();
		/*template<typename T>
		GSerializer& operator<<(T& prop)
		{
			static_assert(	std::is_base_of<GPropertyInterface, std::remove_reference<std::remove_cv<T>::type>::type>::value ||
							std::is_base_of<GObject, std::remove_reference<std::remove_cv<T>::type>::type>::value, "Serializer can serialize only GObject or GPropertyInterface!");
			write(prop);
		}

		template<typename T>
		GSerializer& operator<<(T* prop)
		{
			static_assert(std::is_base_of<GPropertyInterface, std::remove_reference<std::remove_cv<T>::type>::type>::value ||
				std::is_base_of<GObject, std::remove_reference<std::remove_cv<T>::type>::type>::value, "Serializer can serialize only GObject or GPropertyInterface!");
			write(prop);
		}*/

		/*virtual GSerializer& operator<<(GObject& _obj) = 0;
		virtual GSerializer& operator<<(GObject* _obj) = 0;*/
		GSerializer& operator<<(const GPropertyInterface& prop)
		{
			if (prop.isGObjectPointer())
			{
				auto pointer_prop = dynamic_cast<const GPointerPropertyInterface*>(&prop);
				if (pointer_prop->getGObjectPointer())
				{
					postProcessor.addPointer(pointer_prop);
				}
				/*{
					auto itr = reference_providers.find(pointer_prop->getObjectId());
					if (itr == reference_providers.end())
					{
						reference_providers.insert({ pointer_prop->getObjectId(), std::vector<GObjectSharedPtr>({ pointer_prop->getGObjectPointer() }) });
					}
					else
					{
						itr->second.push_back(pointer_prop->getGObjectPointer());
					}
				}*/
			}
			return write(prop);
		}

		GSerializer& operator<<(GObject& prop)
		{
			return write(prop);
		}
		virtual bool writeMetaProperty(const GObject* _obj, GMetaproperty* property) = 0;


	protected:
		virtual GSerializer& write(const GPropertyInterface& property)
		{
			return *this;
		}

		virtual GSerializer& write(const GObject& _obj)
		{
			return *this;
		}

		class PostProcessor
		{
		public:
			void process(GSerializer& serializer);
			void addPointer(const GPointerPropertyInterface* pointer_prop);
			~PostProcessor();
		private:
			std::map<unsigned int, std::vector<GObjectSharedPtr> > reference_providers;
			PostProcessor* childPP = nullptr;
			bool isProcessing = false;
		};
		PostProcessor postProcessor;
		static std::mutex reference_seeker_mutex;
		static std::mutex reference_provider_mutex;
		static std::map<unsigned int, std::vector<GPointerPropertyInterface*> > reference_seekers;
		static std::map<unsigned int, std::vector<GObjectSharedPtr> > reference_providers;
		OStreamSharedPtr stream;
	};

#if 0
	class GFRAMEWORK_API GBinarySerializer : public GSerializer
	{
	public:
		virtual ~GBinarySerializer();
		virtual bool open(OStreamSharedPtr _stream) override;
		virtual bool writeMetaProperty(const GObject* _obj, GMetaproperty* property) override;
		/*virtual GSerializer& operator<<(GObject& _obj) override;
		virtual GSerializer& operator<<(GObject* _obj) override;*/
	};

	class GFRAMEWORK_API GTextSerializer : public GSerializer
	{
	public:
		GTextSerializer();
		virtual ~GTextSerializer();
		virtual bool open(OStreamSharedPtr _stream) override;
		virtual bool writeMetaProperty(const GObject* _obj, GMetaproperty* property) override;
		/*virtual GSerializer& operator<<(GObject& _obj) override;
		virtual GSerializer& operator<<(GObject* _obj) override;*/
	protected:
		const char* objectDelimiter;
	};
#endif
	class GFRAMEWORK_API GDeserializer
	{
	public:
		virtual ~GDeserializer();
		virtual bool open(IStreamSharedPtr _stream);
		IStreamSharedPtr getStream();
		virtual void close();
		//virtual GDeserializer& operator>>(GObject** _obj) = 0;
		virtual bool readMetaProperty(GObjectSharedPtr _obj, GMetaproperty* property) = 0;
		static void addReferenceSeeker(unsigned int _object_id, GObjectSharedPtr* _seeking_object);
		static void addReferenceProviders(unsigned int _object_id, GObjectSharedPtr _providing_object);
		void resolveDependencies();
		void setObject_id(GObjectSharedPtr _obj, uint32 id);
		GDeserializer& operator>>(GPropertyInterface& prop)
		{
			read(prop);
			if (prop.isGObjectPointer())
			{
				auto pointer_prop = dynamic_cast<GPointerPropertyInterface*>(&prop);
				//if (pointer_prop->getObjectId())
				{
					{
						auto itr = reference_seekers.find(pointer_prop->getObjectId());
						if (itr == reference_seekers.end())
						{
							reference_seekers.insert({ pointer_prop->getObjectId(), std::vector<GPointerPropertyInterface*>({ pointer_prop }) });
						}
						else
						{
							itr->second.push_back(pointer_prop);
						}
					}
				}
			}

			return *this;
		}

		GDeserializer& operator>>(GObjectSharedPtr* _obj);

	protected:
		virtual GDeserializer& read(GPropertyInterface& property)
		{
			return *this;
		}

		virtual GDeserializer& read(GObjectSharedPtr* _obj)
		{
			return *this;
		}

		static std::mutex reference_seeker_mutex;
		static std::mutex reference_provider_mutex;
		static std::map<unsigned int, std::vector<GPointerPropertyInterface*> > reference_seekers;
		static std::map<unsigned int, GObjectSharedPtr> reference_providers;
		IStreamSharedPtr stream;
	};
#if 0
	class GFRAMEWORK_API GBinaryDeSerializer : public GDeserializer
	{
	public:
		virtual ~GBinaryDeSerializer();
		virtual bool open(IStreamSharedPtr _stream) override;
		//virtual GDeserializer& operator >> (GObject** _obj) override;
		virtual bool readMetaProperty(GObject* _obj, GMetaproperty* property) override;
	};

	class GFRAMEWORK_API GTextDeSerializer : public GDeserializer
	{
	public:
		GTextDeSerializer();
		virtual ~GTextDeSerializer();
		virtual bool open(IStreamSharedPtr _stream) override;
		//virtual GDeserializer& operator >> (GObject** _obj) override;
		virtual bool readMetaProperty(GObject* _obj, GMetaproperty* property) override;
	protected:
		const char* objectDelimiter;
		virtual std::pair<std::string, std::string> parseProperty(std::string line);
	};
#endif
}