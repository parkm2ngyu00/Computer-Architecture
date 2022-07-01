#include "cache.h"

int tag_shift_bit(int num)
{
	int i;

	i = 0;
	while (num != 1)
	{
		num /= 2;
		i++;
	}
	return (i);
}

int for_index_bit(int n)
{
	int rst;
	if (n == 6)
		return (0);
	rst = (n - 6) * 128 - 64;
	return (rst);
}

int MemRead(int addr, int *MEM, cacheline *cache, COUNT *counter)
{
	int n = tag_shift_bit(cache_size);
	int x = for_index_bit(n);
	int tag = addr >> n;
	int index = (addr & x) >> 6;
	int offset = addr & 0x3f;
	int tmpAddr = addr & 0xffffffc0;
	int dirtyAddr = (cache[index].tag << n | index << 6); // dirty addr
	counter->num_cache_access++;
	// hit
	if ((cache[index].valid) && (cache[index].tag == tag))
	{
		(&cache[index])->sca = 1;
		counter->num_hit++;
		return (cache[index].data[offset / 4]);
	}
	// miss
	else
	{
		counter->num_miss++;
		// cold miss
		if (!cache[index].valid)
		{
			for (int i = 0; i < 16; i++)
			{
				cache[index].data[i] = MEM[tmpAddr / 4 + i];
				printf("0x%08x\n", cache[index].data[i]);
			}
			(&cache[index])->valid = 1;
			(&cache[index])->sca = 0;
			(&cache[index])->dirty = 0;
			(&cache[index])->tag = tag;
			return (cache[index].data[offset / 4]);
		}
		// conflict miss
		else
		{
			if (cache[index].dirty)
			{
				// drain the cacheline
				for (int i = 0; i < 16; i++)
					MEM[dirtyAddr / 4 + i] = cache[index].data[i];
				(&cache[index])->dirty = 0;
			}
			for (int i = 0; i < 16; i++)
				cache[index].data[i] = MEM[tmpAddr / 4 + i];
			(&cache[index])->valid = 1;
			(&cache[index])->sca = 0;
			(&cache[index])->tag = tag;
			return (cache[index].data[offset / 4]);
		}
	}
}

void MemWrite(int addr, int *MEM, int value, cacheline *cache, COUNT *counter)
{
	int n = tag_shift_bit(cache_size);
	int x = for_index_bit(n);
	int tag = addr >> n;
	int index = (addr & x) >> 6;
	int offset = addr & 0x3f;
	int tmpAddr = addr & 0xffffffc0;
	int dirtyAddr = (cache[index].tag << n | index << 6); // dirty addr
	counter->num_cache_access++;
	// hit
	if ((cache[index].valid) && (cache[index].tag == tag))
	{
		counter->num_hit++;
		(&cache[index])->sca = 1;
		(&cache[index])->data[offset / 4] = value;
		(&cache[index])->dirty = 1;
	}
	// miss
	else
	{
		counter->num_miss++;
		// cold miss
		if (!cache[index].valid)
		{
			for (int i = 0; i < 16; i++)
				cache[index].data[i] = MEM[tmpAddr / 4 + i];
			(&cache[index])->valid = 1;
			(&cache[index])->sca = 0;
			(&cache[index])->dirty = 1;
			(&cache[index])->tag = tag;
			(&cache[index])->data[offset / 4] = value;
		}
		// conflict miss
		else
		{
			if (cache[index].dirty)
			{
				for (int i = 0; i < 16; i++)
					MEM[dirtyAddr / 4 + i] = cache[index].data[i];
			}
			for (int i = 0; i < 16; i++)
				(&cache[index])->data[i] = MEM[tmpAddr / 4 + i];
			(&cache[index])->data[offset / 4] = value;
			(&cache[index])->valid = 1;
			(&cache[index])->dirty = 1;
			(&cache[index])->sca = 0;
			(&cache[index])->tag = tag;
		}
	}
}
