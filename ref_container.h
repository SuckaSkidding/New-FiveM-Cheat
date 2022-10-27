// https://github.com/citizenfx/fivem/blob/master/code/client/shared/EventCore.h
#pragma once
#include <atomic>

namespace fx
{
	template<typename T>
	class fwRefContainer
	{
	private:
		T* m_ref;

	public:
		fwRefContainer()
			: m_ref(nullptr)
		{
		}

		fwRefContainer(T* ref)
			: m_ref(ref)
		{
			if (m_ref)
				m_ref->AddRef();
		}

		~fwRefContainer()
		{
			if (m_ref)
			{
				if (m_ref->Release())
					m_ref = nullptr;
			}
		}

		fwRefContainer(const fwRefContainer& rc)
		{
			m_ref = rc.m_ref;

			if (m_ref)
				m_ref->AddRef();
		}

		template<typename TOther>
		fwRefContainer(const fwRefContainer<TOther>& rc)
		{
			m_ref = static_cast<T*>(rc.GetRef());

			if (m_ref)
				m_ref->AddRef();
		}

		uint32_t GetRefCount() const
		{
			return m_ref->GetRefCount();
		}

		T* GetRef() const
		{
			return m_ref;
		}

		T* operator->() const
		{
			return m_ref;
		}

		fwRefContainer& operator=(const fwRefContainer& other)
		{
			if (m_ref)
				m_ref->Release();

			m_ref = other.GetRef();

			if (m_ref)
				m_ref->AddRef();

			return *this;
		}

		template<typename TOther>
		fwRefContainer& operator=(const fwRefContainer<TOther>& other)
		{
			if (m_ref)
				m_ref->Release();

			m_ref = other.GetRef();

			if (m_ref)
				m_ref->AddRef();

			return *this;
		}

		fwRefContainer& operator=(T* ref)
		{
			if (m_ref)
				m_ref->Release();

			m_ref = ref;

			if (m_ref)
				m_ref->AddRef();

			return *this;
		}
	};

	class fwRefCountable
	{
	private:
		class RefCount
		{
		private:
			std::atomic<uint32_t> m_count;

		public:
			RefCount()
				: m_count(0)
			{
			}

			inline std::atomic<uint32_t>& GetCount()
			{
				return m_count;
			}
		};

		RefCount m_refCount;

	public:
		inline uint32_t GetRefCount()
		{
			return m_refCount.GetCount();
		}

		virtual ~fwRefCountable() = 0;

		virtual void AddRef() = 0;

		virtual bool Release() = 0;
	};
} // fx