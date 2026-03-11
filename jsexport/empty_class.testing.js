function onInstantiation(_)
{
	console.log("can construct new Empty(): ", canConstruct(_.Empty));
	console.log("can construct new AllPrivate(): ", canConstruct(_.AllPrivate));
	console.log("can construct new OnlyStatic(): ", canConstruct(_.OnlyStatic));
}
function canConstruct(Class)
{
	let instance;

	try {
		instance = new Class();
	} catch (_) {
		return false;
	}

	instance.delete(); 
	return true;
}
function assert(condition, message)
{
	console.log(message, condition ? "SUCCESS" : "FAILURE");
}
function getPromise(_)
{
	return _.Empty.promise;
}
function getExports()
{
	return {
		get Empty() {
			return Empty;
		},
		get AllPrivate() {
			return AllPrivate;
		},
		get OnlyStatic() {
			return OnlyStatic;
		}
	};
}
function onError(err)
{
	console.log("unexpected exit : FAILURE");
	throw err;
}
