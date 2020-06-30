## [Remove Duplicates from Sorted Arra](https://leetcode.com/problems/remove-duplicates-from-sorted-array/)

Given a sorted array *nums*, remove the duplicates [**in-place**](https://en.wikipedia.org/wiki/In-place_algorithm) such that each element appear only *once* and return the new length.

Do not allocate extra space for another array, you must do this by **modifying the input array [in-place](https://en.wikipedia.org/wiki/In-place_algorithm)** with O(1) extra memory.

**Example 1:**

```
Given nums = [1,1,2],

Your function should return length = 2, with the first two elements of nums being 1 and 2 respectively.

It doesn't matter what you leave beyond the returned length.
```

**Example 2:**

```
Given nums = [0,0,1,1,1,2,2,3,3,4],

Your function should return length = 5, with the first five elements of nums being modified to 0, 1, 2, 3, and 4 respectively.

It doesn't matter what values are set beyond the returned length.
```

```C++
// 效率低的做法，也满足空间复杂度的要求，但是由于数组要进行位置交换，时间复杂度高
class Solution {
public:
	int removeDuplicates(vector<int>& nums) {
		if (nums.size() < 2)
		{
			return nums.size();
		}
		auto pre = nums.front();
		for (vector<int>::iterator iter = nums.begin() + 1; iter != nums.end(); ++iter)
		{
			if (pre == *iter)
			{
				nums.erase(iter); // 调用erase后，iter指向被删除元素的后一个元素
                --iter;
			}
			else
			{
				pre = *iter;
			}
		}
		return nums.size();
	}
};
```

```C++
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if(nums.size() < 2)
        {
            return nums.size();
        }
        int cnt = 1; // 目前有效的数字个数，即一开始第一个数字[0]肯定是有效的
        for(int i = 1; i != nums.size(); ++i)
        {
            if(nums[cnt - 1] != nums[i])
            {
                nums[cnt++] = nums[i];
            }
        }
        return cnt;
    }
};
```

## [Remove Duplicates from Sorted Array II](https://leetcode.com/problems/remove-duplicates-from-sorted-array-ii/)

Given a sorted array *nums*, remove the duplicates [**in-place**](https://en.wikipedia.org/wiki/In-place_algorithm) such that duplicates appeared at most *twice* and return the new length.

Do not allocate extra space for another array, you must do this by **modifying the input array [in-place](https://en.wikipedia.org/wiki/In-place_algorithm)** with O(1) extra memory.

**Example 1:**

```
Given nums = [1,1,1,2,2,3],

Your function should return length = 5, with the first five elements of nums being 1, 1, 2, 2 and 3 respectively.

It doesn't matter what you leave beyond the returned length.
```

**Example 2:**

```
Given nums = [0,0,1,1,1,1,2,3,3],

Your function should return length = 7, with the first seven elements of nums being modified to 0, 0, 1, 1, 2, 3 and 3 respectively.

It doesn't matter what values are set beyond the returned length.
```

```C++
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if(nums.size() < 2)
        {
            return nums.size();
        }
        int cnt = 1; 
        int num_appear_cnt = 1;
        for(int i = 1; i != nums.size(); ++i)
        {
            if(nums[cnt - 1] == nums[i])
            {
                ++num_appear_cnt;
                if(num_appear_cnt <= 2)
                {
                    nums[cnt++] = nums[i];
                }
            }
            if(nums[cnt - 1] != nums[i])
            {
                nums[cnt++] = nums[i];
                num_appear_cnt = 1;
            }
        }
        return cnt;
    }
};
```

## [Longest Consecutive Sequence](https://leetcode.com/problems/longest-consecutive-sequence/)

Given an unsorted array of integers, find the length of the longest consecutive elements sequence.

Your algorithm should run in O(*n*) complexity.

**Example:**

```
Input: [100, 4, 200, 1, 3, 2]
Output: 4
Explanation: The longest consecutive elements sequence is [1, 2, 3, 4]. Therefore its length is 4.
```

```C++
class Solution {
public:
	int longestConsecutive(vector<int>& nums) {
		unordered_set<int> my_set;
		for (auto num : nums)
		{
			my_set.insert(num);
		}
		int maxLen = 0;
		for (auto num : nums)
		{
			int len = 1;
			for (int j = num - 1; my_set.find(j) != my_set.end(); --j)
			{
				++len;
				my_set.erase(j);
			}
			for (int j = num + 1; my_set.find(j) != my_set.end(); ++j)
			{
				++len;
				my_set.erase(j);
			}
			maxLen = max(maxLen, len);
		}
		return maxLen;
	}
};
```

## [Two Sum](https://leetcode.com/problems/two-sum/)

Given an array of integers, return **indices** of the two numbers such that they add up to a specific target.

You may assume that each input would have **exactly** one solution, and you may not use the *same* element twice.

[The function twoSum should return indices of the two numbers such that they add up to the target, where
index1 must be less than index2. Please note that your returned answers (both index1 and index2) are
not zero-based.]

**Example:**

```
Given nums = [2, 7, 11, 15], target = 9,

Because nums[0] + nums[1] = 2 + 7 = 9,
return [0, 1].
```

```C++

class Solution {
public:
	vector<int> twoSum(vector<int>& nums, int target) {
		vector<int> ret;
		unordered_map<int, int> nums_map;
		for (std::size_t i = 0; i != nums.size(); ++i)
		{
			nums_map[nums[i]] = i;
		}
		for (std::size_t i = 0; i != nums.size(); ++i)
		{
			auto rhs = nums_map.find(target - nums[i]);
			if (rhs != nums_map.end() && rhs->second > i)
			{
				ret.push_back(i);
				ret.push_back(rhs->second);
				break;
			}

		}
		return ret;
	}
};

```











## Appendix

### HashTable

通过链地址法解决hash冲突的问题

```C++
#include <iostream>
#include <string>

const std::size_t kHashSize = 16;

struct Node
{
	Node() : next(nullptr) {}

	std::string key;
	std::string value;
	Node *next;
};

class HashTable
{
public:
	// 构造函数，初始化成员变量
	HashTable();

	// 析构函数
	~HashTable();

	// 定义hash函数
	std::size_t Hash(const std::string &key);
	
	// 查找key对应的node
	Node *Lookup(const std::string &key);

	// 插入node
	bool Insert(const std::string &key, const std::string &value);
	
	// 从 hash table 中找到 key 对应的 value
	const std::string Get(const std::string &key);

	// 打印 hash table
	void Display() const;

private:
	Node *m_nodes[kHashSize];
};

HashTable::HashTable()
{
	for (std::size_t i = 0; i != kHashSize; ++i)
	{
		m_nodes[i] = nullptr;
	}
}

HashTable::~HashTable()
{
	for (std::size_t i = 0; i != kHashSize; ++i)
	{
		if (m_nodes[i])
		{
			auto node = m_nodes[i];
			while (node)
			{
				auto temp_node = node->next;
				delete node;
				node = nullptr;
				node = temp_node;
			}
			m_nodes[i] = nullptr;
		}
	}
}

std::size_t HashTable::Hash(const std::string &key)
{
	std::size_t hash_val = 0;
	for(std::size_t i = 0; i != key.size(); ++i)
	{
		hash_val = hash_val * 33 + key[i];
	}
	return hash_val % kHashSize;
}

Node *HashTable::Lookup(const std::string &key)
{
	auto hash_val = Hash(key);
	for (auto node = m_nodes[hash_val]; node; node = node->next)
	{
		if (key == node->key)
		{
			return node;
		}
	}
	return nullptr;
}

bool HashTable::Insert(const std::string &key, const std::string &value)
{
	auto node = Lookup(key);
	if (nullptr == node)
	{
		auto index = Hash(key);
		node = new Node;
		node->key = key;
		node->next = m_nodes[index];
		m_nodes[index] = node;
	}
	node->value = value;
	return true;
}

const std::string HashTable::Get(const std::string &key)
{
	auto node = Lookup(key);
	if (node)
	{
		return node->value;
	}
	else
	{
		return "";
	}
}

void HashTable::Display() const
{
	for (std::size_t i = 0; i != kHashSize; ++i)
	{
		if (m_nodes[i])
		{
			for (auto node = m_nodes[i]; node; node = node->next)
			{
				std::cout << i << ": " << node->key << " " << node->value << std::endl;
			}
		}
	}
}

int main()
{
	{
		HashTable ht;
		std::string s1;

		s1 = "123";
		ht.Insert(s1, "abc");
		s1 = "234";
		ht.Insert(s1, "bcd");
		s1 = "345";
		ht.Insert(s1, "cde");
		s1 = "456";
		ht.Insert(s1, "def");
		s1 = "567";
		ht.Insert(s1, "efg");
		s1 = "678";
		ht.Insert(s1, "fgh");
		s1 = "789";
		ht.Insert(s1, "ghk");
		s1 = "8910";
		ht.Insert(s1, "hkl");
		s1 = "91011";
		ht.Insert(s1, "klm");
		s1 = "101112";
		ht.Insert(s1, "lmn");
		ht.Display();
	}

	return 0;
}
```

