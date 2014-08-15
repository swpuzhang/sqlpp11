/*
 * Copyright (c) 2013-2014, Roland Bock
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *  * Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Sample.h"
#include "MockDb.h"
#include "is_regular.h"
#include <sqlpp11/insert.h>
#include <sqlpp11/functions.h>
#include <iostream>

MockDb db;
MockDb::_serializer_context_t printer;

int main()
{
	test::TabBar t;
	//test::TabFoo f;

	{
		using T = decltype(insert_into(t));
		static_assert(sqlpp::is_regular<T>::value, "type requirement");
	}

	{
		using T = decltype(insert_into(t).set(t.beta = "kirschauflauf"));
		static_assert(sqlpp::is_regular<T>::value, "type requirement");
	}

	{
		using T = decltype(dynamic_insert_into(db, t).dynamic_set());
		static_assert(sqlpp::is_regular<T>::value, "type requirement");
	}

	db(insert_into(t).default_values());
	db(insert_into(t).set(t.beta = "kirschauflauf"));

	serialize(insert_into(t).default_values(), printer).str();

	serialize(insert_into(t), printer).str();
	serialize(insert_into(t).set(t.beta = "kirschauflauf"), printer).str();
	serialize(insert_into(t).columns(t.beta), printer).str();
	auto multi_insert = insert_into(t).columns(t.beta, t.delta);
	multi_insert.values.add(t.beta = "cheesecake", t.delta = 1); 
	multi_insert.values.add(t.beta = sqlpp::default_value, t.delta = sqlpp::default_value); 
	auto i = dynamic_insert_into(db, t).dynamic_set();
	i.insert_list.add(t.beta = "kirschauflauf");
	printer.reset();
	std::cerr << serialize(i, printer).str() << std::endl;


	db(multi_insert);

	db(insert_into(t).set(t.delta = sqlpp::verbatim<sqlpp::integer>("17+4")));
	db(insert_into(t).set(t.delta = sqlpp::null));
	db(insert_into(t).set(t.delta = sqlpp::default_value));

	return 0;
}
