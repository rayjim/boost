#ifndef SC_
#  define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
#endif
   static const boost::array<boost::array<T, 5>, 26> nct_asym = {{
      { SC_(4536808851374080), SC_(0.45368087291717529296875), SC_(-0.481173217296600341796875), SC_(0.1749317497754352460810908541491372795891542325487), SC_(0.8250682502245647539189091458508627204108457674513) }, 
      { SC_(4536808851374080), SC_(0.45368087291717529296875), SC_(1.3978519439697265625), SC_(0.82745888213761221622777144575509830758765026825781), SC_(0.17254111786238778377222855424490169241234973174219) }, 
      { SC_(5677385198338048), SC_(0.56773853302001953125), SC_(-0.5258305072784423828125), SC_(0.13707201688604064907083148328658590547016675560101), SC_(0.86292798311395935092916851671341409452983324439899) }, 
      { SC_(5677385198338048), SC_(0.56773853302001953125), SC_(0.708383023738861083984375), SC_(0.55592460048261968604418681822217801676118984510485), SC_(0.44407539951738031395581318177782198323881015489515) }, 
      { SC_(19057916648620032), SC_(1.905791759490966796875), SC_(3.123167514801025390625), SC_(0.88826935840310006680843037482396692978657176631796), SC_(0.11173064159689993319156962517603307021342823368204) }, 
      { SC_(19057916648620032), SC_(1.905791759490966796875), SC_(3.3844356536865234375), SC_(0.93038224262142835995452986318222481740507172432515), SC_(0.069617757378571640045470136817775182594928275674854) }, 
      { SC_(36700173421772800), SC_(3.670017242431640625), SC_(4.6750431060791015625), SC_(0.84255780194329132905115240305424746317298104246855), SC_(0.15744219805670867094884759694575253682701895753145) }, 
      { SC_(36700173421772800), SC_(3.670017242431640625), SC_(5.042537689208984375), SC_(0.91504926091817108755289153804799207257503305462555), SC_(0.084950739081828912447108461952007927424966945374453) }, 
      { SC_(45079461342740480), SC_(4.507946014404296875), SC_(3.3889064788818359375), SC_(0.13156163646802619543156433884136170088487556815931), SC_(0.86843836353197380456843566115863829911512443184069) }, 
      { SC_(45079461342740480), SC_(4.507946014404296875), SC_(5.9973297119140625), SC_(0.93180682050435927369046460493686411555287073533266), SC_(0.068193179495640726309535395063135884447129264667335) }, 
      { SC_(157509421545553920), SC_(15.750942230224609375), SC_(17.14560699462890625), SC_(0.91844152246389020936377208106238904518645300898785), SC_(0.081558477536109790636227918937610954813546991012154) }, 
      { SC_(157509421545553920), SC_(15.750942230224609375), SC_(17.1575450897216796875), SC_(0.92022740765138483503818882082673489052943410104556), SC_(0.079772592348615164961811179173265109470565898954439) }, 
      { SC_(306140114898124800), SC_(30.614013671875), SC_(31.8541412353515625), SC_(0.89253589230213680868361632253582462638850083853234), SC_(0.10746410769786319131638367746417537361149916146766) }, 
      { SC_(306140114898124800), SC_(30.614013671875), SC_(32.01709747314453125), SC_(0.91970407450026572598532504589599032507374844769296), SC_(0.080295925499734274014674954104009674926251552307043) }, 
      { SC_(390730904542117888), SC_(39.073089599609375), SC_(38.045928955078125), SC_(0.15217241315852103004776349323469720103276253578469), SC_(0.84782758684147896995223650676530279896723746421531) }, 
      { SC_(390730904542117888), SC_(39.073089599609375), SC_(38.2361907958984375), SC_(0.20132472643110592660423913551748708698902279471476), SC_(0.79867527356889407339576086448251291301097720528524) }, 
      { SC_(1044709769224388608), SC_(104.470977783203125), SC_(104.8680572509765625), SC_(0.65434556985043816570960826185482905142969403857975), SC_(0.34565443014956183429039173814517094857030596142025) }, 
      { SC_(1044709769224388608), SC_(104.470977783203125), SC_(105.14849090576171875), SC_(0.75095977650749132960934905238656630719173158415557), SC_(0.24904022349250867039065094761343369280826841584443) }, 
      { SC_(1674453679643557888), SC_(167.44537353515625), SC_(166.869873046875), SC_(0.28247643026063070467278298745564297946418695359089), SC_(0.71752356973936929532721701254435702053581304640911) }, 
      { SC_(1674453679643557888), SC_(167.44537353515625), SC_(168.857147216796875), SC_(0.92099169494016061238522995952802645037178736022794), SC_(0.079008305059839387614770040471973549628212639772055) }, 
      { SC_(2809703283612975104), SC_(280.9703369140625), SC_(279.762969970703125), SC_(0.11364543011784098836896955307019184576823123795544), SC_(0.88635456988215901163103044692980815423176876204456) }, 
      { SC_(2809703283612975104), SC_(280.9703369140625), SC_(282.413665771484375), SC_(0.92553607281289461499104965729721028764390548597727), SC_(0.074463927187105385008950342702789712356094514022729) }, 
      { SC_(7921767423114477568), SC_(792.1767578125), SC_(792.31842041015625), SC_(0.5563267401667033086322021527186481547238720682263), SC_(0.4436732598332966913677978472813518452761279317737) }, 
      { SC_(7921767423114477568), SC_(792.1767578125), SC_(793.54827880859375), SC_(0.91489369852628004292795662749916567238372650186026), SC_(0.085106301473719957072043372500834327616273498139738) }, 
      { SC_(13091821180254420992), SC_(1309.18212890625), SC_(1308.01171875), SC_(0.12091797523015676375913420093212297337117969426545), SC_(0.87908202476984323624086579906787702662882030573455) }, 
      { SC_(13091821180254420992), SC_(1309.18212890625), SC_(1308.517578125), SC_(0.25316892936238242974380182772830364755920164172871), SC_(0.74683107063761757025619817227169635244079835827129) }
   }};
//#undef SC_

