#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

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

int main()
{
	vector<int> nums{ 100, 4, 200, 1, 3, 2 };
	Solution sol;
	sol.longestConsecutive(nums);

	return 0;
}