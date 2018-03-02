import React, { Component } from "react";

var jsonRecipe = '{ \
    "name":"demo", \
    "steps": [ \
        { "type":"weightDoneStep", \
          "goalWeight":7, \
          "info":"7g of Beans!", \
          "tareAfter":true \
        }, \
        { "type":"weightDoneStep", \
          "goalWeight":14, \
          "info":"14g Bloom!", \
          "tareAfter":false \
        }, \
        { "type":"timeDoneStep", \
          "goalTime":10, \
          "info":"ait 10s!", \
          "tareAfter":false \
        }, \
        { "type":"weightTimeDoneMidStep", \
          "goalWeight":100, \
          "startWeight":14, \
          "goalTime":60, \
          "info":"100g Water for 1m!" \
        } ] \
}' ;
var exampleRecipe = JSON.parse(jsonRecipe);
class Recipes extends React.Component {
  constructor() {
    super();
    this.state = {
      items: []
    }
  }


 render() {
   const steps = exampleRecipe.steps
   const stepList = steps.map((step) =>
   <li>{step.info}</li>  
   );
   return (
     <div>
       <h2>GOT QUESTIONS?</h2>
       <p>The easiest thing to do is chill
       </p>
       {stepList}
     </div>
   );
 }
}

export default Recipes