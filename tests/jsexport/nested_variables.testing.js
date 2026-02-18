function onInstantiation(_)
{
	console.log("JS: nestedVariable (expect 3): ", _.Module.nestedVariable);
	console.log("JS: getNestedVariable() (expect 3): ", _.Module.getNestedVariable());
	_.Module.nestedVariable = 4;
	console.log("JS: nestedVariable (expect 4): ", _.Module.nestedVariable);
	console.log("JS: getNestedVariable() (expect 4): ", _.Module.getNestedVariable());
	_.Module.setNestedVariable(5);
	console.log("JS: nestedVariable (expect 5): ", _.Module.nestedVariable);
	console.log("JS: getNestedVariable() (expect 5): ", _.Module.getNestedVariable());
}
function assert(condition, message)
{
	console.log(message, condition ? "SUCCESS" : "FAILURE");
}
function getPromise(_)
{
	return _.Module.getNestedVariable.promise;
}
function getExports()
{
	return {
		get Module() {
			return Module;
		}
	};
}
function onError(err)
{
	console.log("unexpected exit : FAILURE");
	throw err;
}
