
struct Process
{
	unsigned int cpuBurst;

	int id;

	unsigned int remainingTime;

	unsigned int startTime;

	unsigned int waitTime;
};

struct CompareFinishTime
{
	bool operator()(const Process* a, const Process* b) const;

	bool operator()(const Process& a, const Process& b) const;
};

struct CompareRemainingTime
{
	bool operator()(const Process* a, const Process* b) const;

	bool operator()(const Process& a, const Process& b) const;
};

bool compareFinishTime(const Process* a, const Process* b);

bool compareFinishTime(const Process& a, const Process& b);

bool compareRemainingTime(const Process* a, const Process* b);

bool compareRemainingTime(const Process& a, const Process& b);
