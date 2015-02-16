(function()
{
  if(String.prototype.fullStop) return;

  String.prototype.fullStop = function()
  {
    if(/[!;\?]/i.test(this.slice(-1))) return this.trim();

    return this.trim() + '.';
  };
})();
