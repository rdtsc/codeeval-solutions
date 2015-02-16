(function()
{
  if(String.prototype.fullStop) return;

  String.prototype.fullStop = function()
  {
    return this.toString() + ((this.slice(-1) === '.') ? '' : '.');
  };
})();
