function onInstantiation(_)
{
	console.log("JS: staticVariable (expect 3): ", _.Struct.staticVariable);
	console.log("JS: getStaticVariable() (expect 3): ", _.Struct.getStaticVariable());
	_.Struct.staticVariable = 4;
	console.log("JS: staticVariable (expect 4): ", _.Struct.staticVariable);
	console.log("JS: getStaticVariable() (expect 4): ", _.Struct.getStaticVariable());
	_.Struct.setStaticVariable(5)
	console.log("JS: staticVariable (expect 5): ", _.Struct.staticVariable);
	console.log("JS: getStaticVariable() (expect 5): ", _.Struct.getStaticVariable());
}
function assert(condition, message)
{
	console.log(message, condition ? "SUCCESS" : "FAILURE");
}
function getPromise(_)
{
	return _.Struct.promise;
}
function getExports()
{
	return {
		get Struct() {
			return Struct;
		}
	};
}
function onError(err)
{
	console.log("unexpected exit : FAILURE");
	throw err;
}
