#ifndef TRASH_THE_CACHE
#define TRASH_THE_CACHE

struct Transform
{
    float matrix[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1 };
};

class GameObject3D
{
public:
    Transform transform;
    int ID;
};

class GameObject3DAlt
{

public:

    int ID;
    Transform* transform;

};

#include <vector>

class TrashTheCache final
{

public:
    TrashTheCache();
	~TrashTheCache() = default;

	TrashTheCache(const TrashTheCache& other) = delete;
	TrashTheCache(TrashTheCache&& other) = delete;
	TrashTheCache& operator=(const TrashTheCache& other) = delete;
	TrashTheCache& operator=(TrashTheCache&& other) = delete;

	bool RunExercise(int version);

    int& GetSamples();
    float* GetTimingsValues();
    int GetTotalTimings();

private:

    void CalculateAverageTime(const std::vector<long long>& vSamples);
    bool RunVersion1();
    bool RunVersion2();

	const int m_SIZE;		                    // How big is going to be the buffer
    const int m_TOTAL_STEPSIZE;
    int m_Samples;						        // How many samples we want to do     

    static const int m_MAX_TIMINGS = 11;
    float m_FinalTimings[m_MAX_TIMINGS];        // Contains the final average results 
    int m_TimingsIdx;                           // Keep track index we are in the array

}; 

#endif
