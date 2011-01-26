(function() {
  registerafterstart(function() {
    log("hey");
    return go();
  });
}).call(this);
