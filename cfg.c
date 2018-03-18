/* draw control flow graph */

#include <stdio.h>

#include <gcc-plugin.h>
#include <plugin-version.h>
#include <tree.h>
//#include <tree-flow.h>
#include <tree-pass.h>
#include <basic-block.h>
#include <gimple.h>
#include <gimple-pretty-print.h>

static bool my_gate(void)
{
	return true;
}
static unsigned my_exec(void)
{
	basic_block bb;
	edge e;
	edge_iterator ei;

	printf("subgraph fun_%p{\n", cfun);
	
	FOR_ALL_BB(bb) {
		gimple_bb_info *bb_info = &bb->il.gimple;
		printf("bb_%p_%d[label=\"", cfun, bb->index);
		if (bb->index == 0) {
			printf("ENTRY:%s\n%s:%d", function_name(cfun),
					(LOCATION_FILE(cfun->function_start_locus)?:"<unknown>"),
					(LOCATION_LINE(cfun->function_start_locus)));
		} else if (bb->index == 1) {
			printf("EXIT:%s\n%s:%d", function_name(cfun),
					(LOCATION_FILE(cfun->function_end_locus)?:"<unknown>"),
					(LOCATION_LINE(cfun->function_end_locus)));
		} else {
			print_gimple_seq(stdout, bb_info->seq, 0, 0);
		}
		printf("\"];\n");
		FOR_EACH_EDGE(e, ei, bb->succs) {
			basic_block dest = e->dest;
			printf("bb_%p_%d->bb_%p_%d;\n", cfun, bb->index,
					cfun, dest->index);
		}
	}
	printf("}\n");
	return 0;
}
#if 0
static struct gimple_opt_pass my_gcc_plugin_pass = {
	.pass = {
		.type = GIMPLE_PASS,
		.name = "draw_cfg",
		.gate = my_gate,
		.execute = my_exec,
	},
};
#else
static struct gimple_opt_pass my_gcc_plugin_pass;
#endif

void start_graph(void)
{
	printf("digraph cfg {\n");
}
void end_graph(void)
{
	printf("}\n");
}
void finish_gcc(void *gcc_data, void *user_data)
{
	end_graph();
}
static struct plugin_info my_gcc_plugin_info = {
	"1.0",
	"A simple plugin",
};

int plugin_is_GPL_compatible;

int plugin_init(struct plugin_name_args *info, struct plugin_gcc_version *version)
{
	struct register_pass_info pass_info;

	if (!plugin_default_version_check(version, &gcc_version)) {
		printf("gcc version doesn't match\n");
		return 1;
	}
	register_callback(info->base_name, PLUGIN_INFO/*event*/, NULL/*callback*/, &my_gcc_plugin_info/*user data*/);
	my_gcc_plugin_pass.pass.type = GIMPLE_PASS;
	my_gcc_plugin_pass.pass.name ="draw_cfg";
	my_gcc_plugin_pass.pass.gate = my_gate;
	my_gcc_plugin_pass.pass.execute = my_exec;
	pass_info.pass = &my_gcc_plugin_pass.pass;
	pass_info.reference_pass_name = "cfg";
	pass_info.ref_pass_instance_number = 1;
	pass_info.pos_op = PASS_POS_INSERT_AFTER;
	register_callback(info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &pass_info);
	register_callback(info->base_name, PLUGIN_FINISH, finish_gcc, NULL);

	start_graph();

	return 0;
}
