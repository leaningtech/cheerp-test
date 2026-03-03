//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: js, regular

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j | %FileCheck %s

#include <tests.h>
#include <cheerp/client.h>

void webMain()
{
	//Test JS Map
	client::Map<int, int>* tmap = new client::Map<int,int>;

	tmap->set(42, 42);
	tmap->set(45, 74);
	assertPrint("JS TMap::get 1/2:", tmap->get(42));
	// CHECK: JS TMap::get 1/2: 42
	assertPrint("JS TMap::get 2/2:", tmap->get(45));
	// CHECK: JS TMap::get 2/2: 74
	
	assertPrint("JS TMap::get_size() 1/2:", tmap->get_size());
	// CHECK: JS TMap::get_size() 1/2: 2
	tmap->set(45, -42);
	assertPrint("JS TMap::get_size() 2/2:", tmap->get_size());
	// CHECK: JS TMap::get_size() 2/2: 2
	

	assertPrint("JS TMap::has 1/2:", tmap->has(42));
	// CHECK: JS TMap::has 1/2: 1
	assertPrint("JS TMap::has 2/2:", tmap->has(43));
	// CHECK: JS TMap::has 2/2: 0

	assertPrint("JS TMap::get_delete_ 1/2:", tmap->delete_(445));
	// CHECK: JS TMap::get_delete_ 1/2: 0
	assertPrint("JS TMap::get_delete_ 2/2:", tmap->delete_(42));
	// CHECK: JS TMap::get_delete_ 2/2: 1

	tmap->set(1,1);
	tmap->set(100213, 0);
	tmap->set(123123, 23);
	static int sum;
	sum = 0;
	tmap->forEach(cheerp::Callback([](int a, int b){
				sum += a;
				}));
	assertPrint("JS TMap::forEach:", sum);
	// CHECK: JS TMap::forEach: -18
	
	tmap->clear();
	assertPrint("JS TMap::clear:", tmap->get_size());
	// CHECK: JS TMap::clear: 0
}
