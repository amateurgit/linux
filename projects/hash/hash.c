/*
   常用方法:
   链接法     :  数组元素存储指向链表的指针，链表的每个元素都可以存储一个输入的元素
   开放地址法 :  所有的元素都存放在散列表中，它不用指针，是计算出要存取的槽序列。
 */


// BKDR Hash Function
unsigned int BKDRHash(char *str)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;

    while (*str)
    {
	hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

// AP Hash Function
unsigned int APHash(char *str)
{
    unsigned int hash = 0;
    int i;

    for (i=0; *str; i++)
    {
	if ((i & 1) == 0)
	{
	    hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
	}
	else
	{
	    hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
	}
    }

    return (hash & 0x7FFFFFFF);
}


// DJB Hash Function
unsigned int DJBHash(char *str)
{
    unsigned int hash = 5381;

    while (*str)
    {
	hash += (hash << 5) + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

// JS Hash Function
unsigned int JSHash(char *str)
{
    unsigned int hash = 1315423911;

    while (*str)
    {
	hash ^= ((hash << 5) + (*str++) + (hash >> 2));
    }

    return (hash & 0x7FFFFFFF);
}

// RS Hash Function
unsigned int RSHash(char *str)
{
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;

    while (*str)
    {
	hash = hash * a + (*str++);
	a *= b;
    }

    return (hash & 0x7FFFFFFF);
}

unsigned int SDBMHash(char *str)
{
    unsigned int hash = 0;

    while (*str)
    {
	// equivalent to: hash = 65599*hash + (*str++);
	hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }

    return (hash & 0x7FFFFFFF);
}

// P. J. Weinberger Hash Function
unsigned int PJWHash(char *str)
{
    unsigned int BitsInUnignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    unsigned int ThreeQuarters    = (unsigned int)((BitsInUnignedInt  * 3) / 4);
    unsigned int OneEighth        = (unsigned int)(BitsInUnignedInt / 8);
    unsigned int HighBits         = (unsigned int)(0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);
    unsigned int hash             = 0;
    unsigned int test             = 0;

    while (*str)
    {
	hash = (hash << OneEighth) + (*str++);
	if ((test = hash & HighBits) != 0)
	{
	    hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
	}
    }

    return (hash & 0x7FFFFFFF);
}

// ELF Hash Function
unsigned int ELFHash(char *str)
{
    unsigned int hash = 0;
    unsigned int x    = 0;

    while (*str)
    {
	hash = (hash << 4) + (*str++);
	if ((x = hash & 0xF0000000L) != 0)
	{
	    hash ^= (x >> 24);
	    hash &= ~x;
	}
    }

    return (hash & 0x7FFFFFFF);
}



public static int dekhash(String str) {
    int hash = str.length();

    for (int i = 0; i < str.length(); i++) {
	hash = (hash << 5) ^ (hash >> 27) ^ (int)str.charAt(i);
    }

    return hash & 0x7FFFFFFF;
}

public static int bphash(String str) {
    int hash = str.length();

    for (int i = 0; i < str.length(); i++) {
	hash = (hash << 7) ^ (int)str.charAt(i);
    }

    return hash & 0x7FFFFFFF;
}

public static int fnvhash(String str) {
    int fnvprime = 0x811C9DC5;
    int hash = 0;

    for (int i = 0; i < str.length(); i++) {
	hash *= fnvprime;
	hash ^= (int)str.charAt(i);
    }

    return hash & 0x7FFFFFFF;
}

public static int javahash(String str) {
    int hash = 0;

    for (int i = 0; i < str.length(); i++) {
	hash = hash * 31 + (int)str.charAt(i);
    }

    return hash & 0x7FFFFFFF;
}
