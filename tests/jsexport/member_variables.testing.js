function onInstantiation(_)
{
	console.log("JS: memberVariable (expect 3): ", _.getInstance().memberVariable);
	console.log("JS: getMemberVariable (expect 3): ", _.getInstance().getMemberVariable())
	_.getInstance().memberVariable = 4;
	console.log("JS: memberVariable (expect 4): ", _.getInstance().memberVariable);
	console.log("JS: getMemberVariable (expect 4): ", _.getInstance().getMemberVariable())
	_.getInstance().setMemberVariable(5);
	console.log("JS: memberVariable (expect 5): ", _.getInstance().memberVariable);
	console.log("JS: getMemberVariable (expect 5): ", _.getInstance().getMemberVariable())

}
function assert(condition, message)
{
	console.log(message, condition ? "SUCCESS" : "FAILURE");
}
function getPromise(_)
{
	return _.getInstance.promise;
}
function getExports()
{
	return {
		get getInstance() {
			return getInstance;
		}
	};
}
function onError(err)
{
	console.log("unexpected exit : FAILURE");
	throw err;
}
