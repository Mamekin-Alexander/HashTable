#pragma once
#include "hash.h"
#include <list>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <assert.h>

template <class Key, class Value, class Hasher = myHash<Key>>
class myHashMap {
	struct Link {
		Key m_key;
		Value m_value;
	};
	typedef std::list<Link> Chain;
public:
    myHashMap() {
		p = 100;
		generate_a_b();
		m_table.reserve(p);
		for (int32_t i = 0; i < p; ++i)
		{
			m_table.push_back(Chain());
		}
	}
    
    // Feel free to skip these.
    // See smart pointers lab instead, if don't know what they are.
    myHashMap(const myHashMap<Key, Value>& other) = delete;
    myHashMap(myHashMap<Key, Value>&& other) = delete;
    myHashMap& operator=(const myHashMap<Key, Value>& other) = delete;
    myHashMap& operator=(myHashMap<Key, Value>&& other) = delete;
    
    bool Contains(const Key& key) const { 
		bool result = false;
		int64_t index = get_index(key);
		if (!(m_table.at(index).empty()))
		{
			typename Chain::const_iterator i = m_table.at(index).cbegin();
			while (i != m_table.at(index).end())
			{
				if (const_cast<Key&>(i->m_key) == key)
				{
					result = true;
					break;
				}
				++i;
			}
		}
		return result;
	}
    
    void Remove(const Key& key) {
		int64_t index = get_index(key);
		if (!(m_table.at(index).empty()))
		{
			typename std::list<Link>::iterator i = m_table.at(index).begin();
			while (i != m_table.at(index).end())
			{
				if (i->m_key == key)
				{
					m_table.at(index).erase(i);
					break;
				}
				++i;
			}
		}
	}
    
    Value& operator[](const Key& key) {
		int64_t index = get_index(key);
		if (Contains(key))
		{
			typename std::list<Link>::iterator i = m_table.at(index).begin();
			while (i != m_table.at(index).end())
			{
				if (i->m_key == key)
				{
					return(i->m_value);
				}
				++i;
			}
		}
		else
		{
			m_table.at(index).push_back(Link{ key, NULL });
			return m_table.at(index).back().m_value;
		}
	}
    
    ~myHashMap() {
		a = NULL;
		b = NULL;
		p = NULL;
		m_table.clear();
    }
    
private:
	void generate_a_b()
	{
		std::srand(static_cast<unsigned int>(time(0)));
		a = std::rand() % p;
		b = std::rand() % p;
	}
	int64_t get_index(const Key& key) const
	{
		return (a + b * m_hasher(key)) % p;
	}
	int64_t a, b, p;
	std::vector<Chain> m_table;
	Hasher m_hasher;
};
