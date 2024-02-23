#include "json.c"

int main() {
	FILE *f = fopen("../src/main/assets/web.json", "rb");
	//FILE *f = fopen("test.json", "rb");
	if (f == NULL) return -1;

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *default_bible = malloc(fsize + 1);
	fread(default_bible, fsize, 1, f);
	fclose(f);
	default_bible[fsize] = 0;

	struct Parse p = {0};

	json_get(&p, default_bible, "['osis']['osisText']['header']['work']['title']");
	puts(json_fixup_string(&p));

	json_get(&p, default_bible, "['osis']['osisText']['header']['work']['description']");
	puts(json_fixup_string(&p));

	json_get(&p, default_bible, "['osis']['osisText']['header']['work']['publisher']");
	puts(json_fixup_string(&p));

	// Navigate to Revelation 1, save the 'checkpoint'
	json_get(&p, default_bible, "['osis']['osisText']['div'][30]['chapter']['verse']");
	puts(json_fixup_string(&p));
	void *checkpoint = p.b;

	// Rev 1 1
	json_get(&p, checkpoint, "[0][1]");
	puts(json_fixup_string(&p));

	// Rev 1 2
	json_get(&p, checkpoint, "[14][1]");
	puts(json_fixup_string(&p));

	return 0;
}
