(function()
{
  if(String.prototype.capitalize) return;

  String.prototype.capitalize = function()
  {
    return this.toLowerCase().replace(/\b\w/g, function(letter)
    {
      return letter.toUpperCase();
    });
  };
})();
