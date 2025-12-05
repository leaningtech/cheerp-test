function onInstantiation(_)
{
	let base = new _.Base();
	let derived = new _.Derived();

	console.log("base memberBaseTest (expect 1): ", base.memberBaseTest());
	console.log("base memberShadowTest (expect 0): ", base.memberShadowTest());
	console.log("base virtualTest (expect 0): ", base.virtualTest());
	console.log("base pureVirtualTest (expect 0): ", base.pureVirtualTest());

	console.log("derived memberBaseTest (expect 1): ", derived.memberBaseTest());
	console.log("derived memberDerivedTest (expect 1):", derived.memberDerivedTest());
	console.log("derived memberShadowTest (expect 1): ", derived.memberShadowTest());
	console.log("derived virtualTest (expect 1): ", derived.virtualTest());
	console.log("derived pureVirtualTest (expect 1): ", derived.pureVirtualTest());

	console.log("base staticMemberTest (expect 1): ", _.Base.staticBaseTest());
	console.log("base StaticShadowTest (expect 0): ", _.Base.staticShadowTest());

	console.log("derived staticDerivedTest (expect 1): ", _.Derived.staticDerivedTest());
	console.log("derived staticShadowTest (expect 1): ", _.Derived.staticShadowTest());

	console.log("base instanceof Derived (expect false): ", base instanceof _.Derived);
	console.log("derived instanceof Base (expect true): ", derived instanceof _.Base) 

	base.delete();
	derived.delete();
}
function assert(condition, message)
{
	console.log(message, condition ? "SUCCESS" : "FAILURE");
}
function getPromise(_)
{
	return _.Base.promise;
}
function getExports()
{
	return {
		get Base() {
			return Base;
		},
		get Derived() {
			return Derived;
		}
	};
}
function onError(err)
{
	console.log("unexpected exit : FAILURE");
	throw err;
}
