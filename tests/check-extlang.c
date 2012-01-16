/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/* 
 * check-extlang.c
 * Copyright (C) 2011-2012 Akira TAGOH
 * 
 * Authors:
 *   Akira TAGOH  <akira@tagoh.org>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liblangtag/langtag.h>
#include "main.h"

static lt_extlang_db_t *db;

/************************************************************/
/* common functions                                         */
/************************************************************/
void
setup(void)
{
	db = lt_db_get_extlang();
}

void
teardown(void)
{
	lt_extlang_db_unref(db);
}

/************************************************************/
/* Test cases                                               */
/************************************************************/
TDEF (lt_extlang_compare) {
	lt_extlang_t *e1, *e2;

	e1 = lt_extlang_db_lookup(db, "yue");
	fail_unless(e1 != NULL, "No expected extlang found: 'yue'");
	fail_unless(lt_extlang_compare(e1, e1), "extlang that points to the same object should be dealt as same");
	e2 = lt_extlang_db_lookup(db, "hks");
	fail_unless(e2 != NULL, "No expected extlang found: 'hks'");
	fail_unless(!lt_extlang_compare(e1, e2), "extlang that has different tag should be dealt as different.");
	lt_extlang_unref(e2);
	e2 = lt_extlang_db_lookup(db, "*");
	fail_unless(e2 != NULL, "No expected extlang found: '*'");
	fail_unless(lt_extlang_compare(e1, e2), "wildcard should be matched with any objects.");
	lt_extlang_unref(e2);
	e2 = lt_extlang_db_lookup(db, "");
	fail_unless(e2 != NULL, "No expected extlang found: ''");
	fail_unless(!lt_extlang_compare(e1, e2), "extlang that has different tag should be dealt as different even if it's an empty entry.");
	lt_extlang_unref(e2);
	lt_extlang_unref(e1);
} TEND

/************************************************************/
Suite *
tester_suite(void)
{
	Suite *s = suite_create("lt_extlang_t");
	TCase *tc = tcase_create("Basic functionality");

	tcase_add_checked_fixture(tc, setup, teardown);

	T (lt_extlang_compare);

	suite_add_tcase(s, tc);

	return s;
}
