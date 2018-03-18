enum xx {
	HI,
	OK,
};
struct a {
	enum xx a;
	int b;
};
struct abc {
	struct a a;
};

int test(void)
{
	return 1;
}
int main(void)
{
#if gcc
	struct abc x = {
		.a.a = 1,
		.a.b = 2,
	};
#else
	struct abc x = {
		.a = {
			.a = HI,
			.b = test(),
		},
	};
#endif
	return 0;
}
