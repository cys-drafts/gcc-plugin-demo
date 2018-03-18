#include <stdio.h>

#include <gcc-plugin.h>
#include <plugin-version.h>

int plugin_is_GPL_compatible;

int plugin_init(struct plugin_name_args *info, struct plugin_gcc_version *version)
{
	int i;

	if (!plugin_default_version_check(version, &gcc_version)) {
		printf("version doesn't match\n");
		return 1;
	}
	printf("plugin info:\n");
	if (info->version)
		printf("version %s\n", info->version);
	if (info->help)
		printf("help %s\n", info->help);
	printf("basename %s\n", info->base_name);
	printf("fullname %s\n", info->full_name);
	printf("args %d\n", info->argc);
	for (i = 0; i < info->argc; i++) {
		printf("arg-%d %s %s\n", i, info->argv[i].key, info->argv[i].value);
	}
	printf("\n");
	printf("version info:\n");
	printf("base version %s\n", version->basever);
	printf("date stamp %s\n", version->datestamp);
	printf("dev phase %s\n", version->devphase);
	printf("revison %s\n", version->revision);
	printf("configuration arguments %s\n", version->configuration_arguments);
	printf("\n");

	printf("plugin initialised successfully\n");

	return 0;
}

