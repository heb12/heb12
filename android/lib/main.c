#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ui.h>
#include "json.h"
#include "myjni.h"
#include "bible.h"

void display_chapter(char *bible, int book, int chapter);

struct Heb12App {
	char *bible;
	int book;
	int chapter;
}app = {
	.chapter = 0,
	.book = 0,
};

uiBox *verse_box = NULL;
uiCombobox *book_dropdown = NULL;
uiCombobox *chapter_dropdown = NULL;

void plat_dbg(char *fmt, ...) {
	char buffer[512];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	__android_log_write(ANDROID_LOG_ERROR, "heb12", buffer);
}

static void update_chapters() {
	uiComboboxClear(chapter_dropdown);
	for (size_t i = 0; i < Bible_info.books[app.book].chapter; i++) {
		char buffer[8];
		sprintf(buffer, "%d", i + 1);
		uiComboboxAppend(chapter_dropdown, buffer);
	}
};

static void click_next(uiButton *btn, void *data) {
	if (app.chapter >= Bible_info.books[app.book].chapter - 1) {
		if (app.book == BIBLE_BOOKS - 1) {
			app.book = 0;
		} else {
			app.book++;
		}
		app.chapter = 0;
		update_chapters();
		uiComboboxSetSelected(book_dropdown, app.book);
	} else {
		app.chapter++;
	}
	display_chapter(app.bible, app.book, app.chapter);
	uiComboboxSetSelected(chapter_dropdown, app.chapter);
}

static void click_previous(uiButton *btn, void *data) {
	if (app.chapter == 0) {
		if (app.book == 0) {
			app.book = BIBLE_BOOKS - 1;
		} else {
			app.book--;
		}
		app.chapter = Bible_info.books[app.book].chapter - 1;
		update_chapters();
		uiComboboxSetSelected(book_dropdown, app.book);
	} else {
		app.chapter--;
	}
	display_chapter(app.bible, app.book, app.chapter);
	uiComboboxSetSelected(chapter_dropdown, app.chapter);
}

int json_getf(struct Parse *p, void *checkpoint, char *fmt, ...) {
	char buffer[512];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	json_get(p, checkpoint, buffer);
	return p->out_of_bounds;
}

void set_verse(int index, char *init_text) {
	uiControl *verse;
	if (index >= uiBoxNumChildren(verse_box)) {
		verse = uiExpandControl("verse");
		uiBoxAppend(verse_box, verse, 0);
	} else {
		verse = uiBoxChild(verse_box, index);
	}

	uiLabel *verse_text = (uiLabel *)uiBoxChild((uiBox *)verse, 1);
	uiLabelSetText(verse_text, init_text);

	uiLabel *verse_number = (uiLabel *)uiBoxChild((uiBox *)verse, 0);

	char buffer[16];
	sprintf(buffer, "%d", index + 1);
	uiLabelSetText(verse_number, buffer);
}

void display_chapter(char *bible, int book, int chapter) {
	// plat_dbg("Showing %d/%d", book, chapter);
	struct Parse p = {0};
	// Obadiah, Philemon, 2 & 3 John, Jude have one chapter, different structure. This works.
	if (chapter == 30 || chapter == 56 || chapter == 63 || chapter == 64 || chapter == 65) {
		json_getf(&p, bible, "['osis']['osisText']['div'][%d]['chapter']['verse']", book);
	} else {
		json_getf(&p, bible, "['osis']['osisText']['div'][%d]['chapter'][%d]['verse']", book, chapter);
	}

	void *checkpoint = p.b;

	char *verse_text = NULL;
	int i;
	for (i = 0; 1; i++) {
		json_getf(&p, checkpoint, "[%d][1]", i);
		if (p.out_of_bounds) break;
		verse_text = json_fixup_string(&p);
		set_verse(i, verse_text);
		free(verse_text);
	}

	// Remove excess labels
	int left = uiBoxNumChildren(verse_box);
	int row = i;
	for (; i < left; i++) {
		uiControl *ctl = uiBoxChild(verse_box, row);
		uiBoxDelete(verse_box, row);
		uiControlDestroy(ctl);
	}
}

static void book_selected(uiCombobox *c, void *data) {
	app.book = uiComboboxSelected(c);
	display_chapter(app.bible, app.book, app.chapter);
}

static void chapter_selected(uiCombobox *c, void *data) {
	int chp = uiComboboxSelected(c);
	if (chp == app.chapter) return;
	app.chapter = chp;
	display_chapter(app.bible, app.book, app.chapter);
}

JNI_FUNC(void, StartUI)(JNIEnv *env, jobject thiz, jobject ctx) {
	if (uiAndroidInit(env, ctx)) abort();


	verse_box = (uiBox *)uiControlFromID("verse_box");

	book_dropdown = (uiCombobox *)uiControlFromID("book_spinner");
	chapter_dropdown = (uiCombobox *)uiControlFromID("chapter_spinner");

	uiComboboxOnSelected(book_dropdown, book_selected, NULL);
	uiComboboxOnSelected(chapter_dropdown, chapter_selected, NULL);

	for (size_t i = 0; i < BIBLE_BOOKS; i++) {
		uiComboboxAppend(book_dropdown, Bible_info.books[i].name);
	}

	update_chapters();

	uiButton *btn = (uiButton *)uiControlFromID("btn_next");
	uiButtonOnClicked(btn, click_next, NULL);

	uiButton *btn_prev = (uiButton *)uiControlFromID("btn_prev");
	uiButtonSetText(btn_prev, uiGet("prev"));
	uiButtonOnClicked(btn_prev, click_previous, NULL);

	int length;
	app.bible = libu_get_assets_file(env, uiAndroidGetCtx(), "web.json", &length);

	display_chapter(app.bible, 0, 0);

	uiControl *ctl = uiBoxChild(verse_box, 0);
	uiControlDestroy(ctl);
}

