function onInstantiation(_)
{
	// Only use accessor functions - globalVariable is not directly exported
	console.log("JS: getGlobalVariable() (expect 3): ", _.getGlobalVariable());
	_.setGlobalVariable(4);
	console.log("JS: getGlobalVariable() (expect 4): ", _.getGlobalVariable());
	_.setGlobalVariable(5);
	console.log("JS: getGlobalVariable() (expect 5): ", _.getGlobalVariable());
}
function assert(condition, message)
{
	console.log(message, condition ? "SUCCESS" : "FAILURE");
}
function getPromise(_)
{
	return _.getGlobalVariable.promise;
}
function getExports()
{
	// Return the actual exported functions
	return {
		getGlobalVariable: getGlobalVariable,
		setGlobalVariable: setGlobalVariable
	};
}
function onError(err)
{
	console.log("unexpected exit : FAILURE");
	throw err;
}
