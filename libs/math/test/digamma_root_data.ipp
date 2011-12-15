//  (C) Copyright John Maddock 2006-7.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

   static const boost::array<boost::array<T, 2>, 200> digamma_root_data = { {
      {{ SC_(1.39999997615814208984375), SC_(-0.06138456903152256550686860248931989075643) }}, 
      {{ SC_(1.4005000591278076171875), SC_(-0.06087192929516339380130503872301167293378) }}, 
      {{ SC_(1.40100002288818359375), SC_(-0.06035965880979232302674791723368356347747) }}, 
      {{ SC_(1.4014999866485595703125), SC_(-0.05984763511635311349827564040488766194063) }}, 
      {{ SC_(1.401999950408935546875), SC_(-0.0593358579881174546738279942752396761834) }}, 
      {{ SC_(1.40250003337860107421875), SC_(-0.05882420526073090100751004662830533539728) }}, 
      {{ SC_(1.40299999713897705078125), SC_(-0.0583129206425757749570876729240439746001) }}, 
      {{ SC_(1.40349996089935302734375), SC_(-0.05780188191118900126815738602890619172921) }}, 
      {{ SC_(1.4040000438690185546875), SC_(-0.05729096707893393080771549505001408983822) }}, 
      {{ SC_(1.40450000762939453125), SC_(-0.056780419503293858705549580743156540668) }}, 
      {{ SC_(1.4049999713897705078125), SC_(-0.05627011713868186213939939894956276866569) }}, 
      {{ SC_(1.40550005435943603515625), SC_(-0.05575993817369138452985739956960740694737) }}, 
      {{ SC_(1.40600001811981201171875), SC_(-0.05525012561586888099905375034247029300063) }}, 
      {{ SC_(1.40649998188018798828125), SC_(-0.05474055759601525571890189603476879643263) }}, 
      {{ SC_(1.40699994564056396484375), SC_(-0.05423123389038862937136671914178719811082) }}, 
      {{ SC_(1.4075000286102294921875), SC_(-0.05372203292179596750107248206039107172611) }}, 
      {{ SC_(1.40799999237060546875), SC_(-0.05321319723270065414202667836306539722927) }}, 
      {{ SC_(1.4084999561309814453125), SC_(-0.05270460518832646256646398238653334023619) }}, 
      {{ SC_(1.40900003910064697265625), SC_(-0.05219613538658003557049855371629116390416) }}, 
      {{ SC_(1.40950000286102294921875), SC_(-0.05168803002221822433984269085445887545932) }}, 
      {{ SC_(1.40999996662139892578125), SC_(-0.05118016763572077571206202324130731620467) }}, 
      {{ SC_(1.410500049591064453125), SC_(-0.05067242699961275111564488922723020894775) }}, 
      {{ SC_(1.4110000133514404296875), SC_(-0.05016504996189404179318756205383368841508) }}, 
      {{ SC_(1.41149997711181640625), SC_(-0.04965791523781849836663795623050040651275) }}, 
      {{ SC_(1.41200006008148193359375), SC_(-0.04915090177405025431768617393574718923719) }}, 
      {{ SC_(1.41250002384185791015625), SC_(-0.04864425107277807126795345488864174180773) }}, 
      {{ SC_(1.41299998760223388671875), SC_(-0.04813784202354994896630230505781776120276) }}, 
      {{ SC_(1.41349995136260986328125), SC_(-0.04763167440643069181645608937840354367671) }}, 
      {{ SC_(1.414000034332275390625), SC_(-0.04712562739952290889837033293650476187953) }}, 
      {{ SC_(1.4144999980926513671875), SC_(-0.04661994204540782942709091596942293402734) }}, 
      {{ SC_(1.41499996185302734375), SC_(-0.04611449746527775349715574804732130222007) }}, 
      {{ SC_(1.41550004482269287109375), SC_(-0.04560917301026328003252147788241984436241) }}, 
      {{ SC_(1.41600000858306884765625), SC_(-0.0451042093793269736581833678634701785191) }}, 
      {{ SC_(1.41649997234344482421875), SC_(-0.04459948586684279443839061743784846261709) }}, 
      {{ SC_(1.4170000553131103515625), SC_(-0.04409488199649628730133723954943974407746) }}, 
      {{ SC_(1.417500019073486328125), SC_(-0.0435906381245648444628637002942433530947) }}, 
      {{ SC_(1.4179999828338623046875), SC_(-0.04308663371813044418132048918710682813622) }}, 
      {{ SC_(1.41849994659423828125), SC_(-0.04258286856012920630307449298163993137931) }}, 
      {{ SC_(1.41900002956390380859375), SC_(-0.0420792224035827733504570446125334946695) }}, 
      {{ SC_(1.41949999332427978515625), SC_(-0.04157593514930978152490152156879729835277) }}, 
      {{ SC_(1.41999995708465576171875), SC_(-0.04107288649393147397031813049746661830252) }}, 
      {{ SC_(1.4205000400543212890625), SC_(-0.04056995636192793782845393152872471220703) }}, 
      {{ SC_(1.421000003814697265625), SC_(-0.04006738431359655642636465412836862063583) }}, 
      {{ SC_(1.4214999675750732421875), SC_(-0.03956505021716870619391607561977829898393) }}, 
      {{ SC_(1.42200005054473876953125), SC_(-0.0390628341681160707895772877982347062534) }}, 
      {{ SC_(1.42250001430511474609375), SC_(-0.03856097538713614221174291451172868720556) }}, 
      {{ SC_(1.42299997806549072265625), SC_(-0.03805935391360299407483745199209552218662) }}, 
      {{ SC_(1.42349994182586669921875), SC_(-0.03755796953327570443031191372677272436563) }}, 
      {{ SC_(1.4240000247955322265625), SC_(-0.03705670256888721277125707475995514593767) }}, 
      {{ SC_(1.424499988555908203125), SC_(-0.03655579178977111826937882476062071563405) }}, 
      {{ SC_(1.4249999523162841796875), SC_(-0.0360551174627633043738925866730974379965) }}, 
      {{ SC_(1.42550003528594970703125), SC_(-0.03555456008050613945891518063349475611711) }}, 
      {{ SC_(1.42599999904632568359375), SC_(-0.03505435807486918460538076814912383388358) }}, 
      {{ SC_(1.42649996280670166015625), SC_(-0.03455439188274749223938775275471736825604) }}, 
      {{ SC_(1.4270000457763671875), SC_(-0.03405454216623210245980763665400513878778) }}, 
      {{ SC_(1.4275000095367431640625), SC_(-0.03355504702063797243050550756083487574355) }}, 
      {{ SC_(1.427999973297119140625), SC_(-0.03305578705245798813618865616700236433431) }}, 
      {{ SC_(1.42850005626678466796875), SC_(-0.03255664309277291143229018160781330281834) }}, 
      {{ SC_(1.42900002002716064453125), SC_(-0.03205785290124899051424545775403784997954) }}, 
      {{ SC_(1.42949998378753662109375), SC_(-0.03155929725351741957783226833498005852699) }}, 
      {{ SC_(1.42999994754791259765625), SC_(-0.03106097593893646101388889322754816060391) }}, 
      {{ SC_(1.430500030517578125), SC_(-0.03056277001319123536849588910788667969385) }}, 
      {{ SC_(1.4309999942779541015625), SC_(-0.03006491678982831278985395858099735606325) }}, 
      {{ SC_(1.431499958038330078125), SC_(-0.02956729726928046744375226173805018083445) }}, 
      {{ SC_(1.43200004100799560546875), SC_(-0.029069792675158646894282928074400495447) }}, 
      {{ SC_(1.43250000476837158203125), SC_(-0.02857263998746077965589217391685781750225) }}, 
      {{ SC_(1.43299996852874755859375), SC_(-0.02807572037469272194244519088403794652932) }}, 
      {{ SC_(1.4335000514984130859375), SC_(-0.02757891522793825630459486913227966938778) }}, 
      {{ SC_(1.4340000152587890625), SC_(-0.02708246119454089526602248424490897334451) }}, 
      {{ SC_(1.4344999790191650390625), SC_(-0.02658623961062601916105249666629939819311) }}, 
      {{ SC_(1.434999942779541015625), SC_(-0.02609025026826734736532129982459892559412) }}, 
      {{ SC_(1.43550002574920654296875), SC_(-0.02559437478113976107312669529902398957573) }}, 
      {{ SC_(1.43599998950958251953125), SC_(-0.0250988493544413555894730759899566827115) }}, 
      {{ SC_(1.43649995326995849609375), SC_(-0.02460355554708331611314729590329885967853) }}, 
      {{ SC_(1.4370000362396240234375), SC_(-0.02410837513916324870453956495027706651788) }}, 
      {{ SC_(1.4375), SC_(-0.02361354400529789914172989421521805262163) }}, 
      {{ SC_(1.4379999637603759765625), SC_(-0.02311894387096655710370815673046310749581) }}, 
      {{ SC_(1.43850004673004150390625), SC_(-0.02262445668224241951726505286548155427726) }}, 
      {{ SC_(1.43900001049041748046875), SC_(-0.02213031798404363285679840262837603735113) }}, 
      {{ SC_(1.43949997425079345703125), SC_(-0.02163640966796990067907296550632756430219) }}, 
      {{ SC_(1.440000057220458984375), SC_(-0.02114261384562461502572001614025897932072) }}, 
      {{ SC_(1.4405000209808349609375), SC_(-0.02064916573310666785738104392419687958556) }}, 
      {{ SC_(1.4409999847412109375), SC_(-0.02015594738769022700601666231128218163721) }}, 
      {{ SC_(1.4414999485015869140625), SC_(-0.0196629586049117094653247721121789058042) }}, 
      {{ SC_(1.44200003147125244140625), SC_(-0.0191700817163872207492164657910951176529) }}, 
      {{ SC_(1.44249999523162841796875), SC_(-0.01867755150116066387436310533382643774849) }}, 
      {{ SC_(1.44299995899200439453125), SC_(-0.01818525023671041840942493794786294366844) }}, 
      {{ SC_(1.443500041961669921875), SC_(-0.01769306041915025068329883160626130368828) }}, 
      {{ SC_(1.4440000057220458984375), SC_(-0.01720121650074344257459977191565623298452) }}, 
      {{ SC_(1.444499969482421875), SC_(-0.01670960092360902193107619746814218063958) }}, 
      {{ SC_(1.44500005245208740234375), SC_(-0.01621809634791875134279906903350577844016) }}, 
      {{ SC_(1.44550001621246337890625), SC_(-0.015726936900023407129612092249722223488) }}, 
      {{ SC_(1.44599997997283935546875), SC_(-0.01523600518624245606690212546401818985812) }}, 
      {{ SC_(1.44649994373321533203125), SC_(-0.01474530100472522827513829838650753549117) }}, 
      {{ SC_(1.447000026702880859375), SC_(-0.01425470723370217219431406682491211640879) }}, 
      {{ SC_(1.4474999904632568359375), SC_(-0.01376445756632729291905843124879046109968) }}, 
      {{ SC_(1.4479999542236328125), SC_(-0.01327443482716782005100353254837490055862) }}, 
      {{ SC_(1.44850003719329833984375), SC_(-0.01278452205749292497551903285885539659936) }}, 
      {{ SC_(1.44900000095367431640625), SC_(-0.01229495262655675620536025426734759387572) }}, 
      {{ SC_(1.44949996471405029296875), SC_(-0.01180560952210644817133945874679139627505) }}, 
      {{ SC_(1.4500000476837158203125), SC_(-0.01131637594801586656260394588481713457627) }}, 
      {{ SC_(1.450500011444091796875), SC_(-0.01082748495049637469227173996198819829549) }}, 
      {{ SC_(1.4509999752044677734375), SC_(-0.01033881968004044725477451816247304271862) }}, 
      {{ SC_(1.45150005817413330078125), SC_(-0.009850263502694288097376018889413894603933) }}, 
      {{ SC_(1.45200002193450927734375), SC_(-0.009362049142480268951706194627385896301701) }}, 
      {{ SC_(1.45249998569488525390625), SC_(-0.008874059912203315333901183590153038939381) }}, 
      {{ SC_(1.45299994945526123046875), SC_(-0.008386295613345236665485880245372577446211) }}, 
      {{ SC_(1.4535000324249267578125), SC_(-0.007898639827508066919974235979462981148542) }}, 
      {{ SC_(1.453999996185302734375), SC_(-0.007411324850459837999980330364290265721957) }}, 
      {{ SC_(1.4544999599456787109375), SC_(-0.006924234210747293687116178727328347532758) }}, 
      {{ SC_(1.45500004291534423828125), SC_(-0.006437251651142443774260894146878774165176) }}, 
      {{ SC_(1.45550000667572021484375), SC_(-0.005950609147203835378592212115736517701905) }}, 
      {{ SC_(1.45599997043609619140625), SC_(-0.005464190388787066551639512688711491505847) }}, 
      {{ SC_(1.45650005340576171875), SC_(-0.00497787927940728167871412685817772062165) }}, 
      {{ SC_(1.4570000171661376953125), SC_(-0.004491907475256527500473978270689662215921) }}, 
      {{ SC_(1.457499980926513671875), SC_(-0.004006158827071890461464755144012668001093) }}, 
      {{ SC_(1.4579999446868896484375), SC_(-0.003520633138850239630378016887483403867021) }}, 
      {{ SC_(1.45850002765655517578125), SC_(-0.003035214527765513668034604216830692388851) }}, 
      {{ SC_(1.45899999141693115234375), SC_(-0.002550134225501944697075651923125347621858) }}, 
      {{ SC_(1.45949995517730712890625), SC_(-0.002065276296639069833107196292495616468625) }}, 
      {{ SC_(1.46000003814697265625), SC_(-0.001580525018104361453955140711216969645092) }}, 
      {{ SC_(1.4605000019073486328125), SC_(-0.001096111304170043846745287256190494732511) }}, 
      {{ SC_(1.460999965667724609375), SC_(-0.0006119193793063274064283424010699749479896) }}, 
      {{ SC_(1.46150004863739013671875), SC_(-0.0001278336797728010038687456086772293940197) }}, 
      {{ SC_(1.46200001239776611328125), SC_(0.0003559151967379670943728162750581078035161) }}, 
      {{ SC_(1.46249997615814208984375), SC_(0.0008394428662870266119150633759096763287959) }}, 
      {{ SC_(1.4630000591278076171875), SC_(0.001322864733702479649625580945291659569672) }}, 
      {{ SC_(1.46350002288818359375), SC_(0.001805950517042218036842926190553563525984) }}, 
      {{ SC_(1.4639999866485595703125), SC_(0.002288815673318872744259324682705288271519) }}, 
      {{ SC_(1.464499950408935546875), SC_(0.002771460395327381562525270989780009242693) }}, 
      {{ SC_(1.46500003337860107421875), SC_(0.003253999876667223767472091225924873652768) }}, 
      {{ SC_(1.46549999713897705078125), SC_(0.003736204255104145549500401970100669234964) }}, 
      {{ SC_(1.46599996089935302734375), SC_(0.004218188776241473723916466805139222942859) }}, 
      {{ SC_(1.4665000438690185546875), SC_(0.004700068475739702789490158181882191908657) }}, 
      {{ SC_(1.46700000762939453125), SC_(0.005181613805192456365413438059635728009745) }}, 
      {{ SC_(1.4674999713897705078125), SC_(0.005662939852129155621931915752726712941002) }}, 
      {{ SC_(1.46800005435943603515625), SC_(0.006144161494685809209920008019685283315534) }}, 
      {{ SC_(1.46850001811981201171875), SC_(0.006625049497465130483020027415596322710511) }}, 
      {{ SC_(1.46899998188018798828125), SC_(0.007105718790337639298786852855563148822142) }}, 
      {{ SC_(1.46949994564056396484375), SC_(0.007586169563676627501200448950668213028167) }}, 
      {{ SC_(1.4700000286102294921875), SC_(0.008066516486231662885434708649134127678999) }}, 
      {{ SC_(1.47049999237060546875), SC_(0.008546530738674327360836097967927454942833) }}, 
      {{ SC_(1.4709999561309814453125), SC_(0.00902632704131026480217858186061492831619) }}, 
      {{ SC_(1.47150003910064697265625), SC_(0.009506019906325596650154195641432409630907) }}, 
      {{ SC_(1.47200000286102294921875), SC_(0.009985380825502499855688695923101130679945) }}, 
      {{ SC_(1.47249996662139892578125), SC_(0.01046452436244992624581900727741018711986) }}, 
      {{ SC_(1.473000049591064453125), SC_(0.01094356487319982808059040901671966801919) }}, 
      {{ SC_(1.4735000133514404296875), SC_(0.01142227415983642922640932055783984854239) }}, 
      {{ SC_(1.47399997711181640625), SC_(0.01190076662968176394047411758437134448755) }}, 
      {{ SC_(1.47450006008148193359375), SC_(0.01237915648302134259978832581154852202161) }}, 
      {{ SC_(1.47500002384185791015625), SC_(0.01285721583143592736486164760948599640939) }}, 
      {{ SC_(1.47549998760223388671875), SC_(0.01333505892636885712587736290231507253031) }}, 
      {{ SC_(1.47599995136260986328125), SC_(0.01381268595510390028319865656189032981603) }}, 
      {{ SC_(1.476500034332275390625), SC_(0.01429021091090032875378729920511744436802) }}, 
      {{ SC_(1.4769999980926513671875), SC_(0.01476740631674500824066000704296476101248) }}, 
      {{ SC_(1.47749996185302734375), SC_(0.01524438621687947040773770812918073015903) }}, 
      {{ SC_(1.47800004482269287109375), SC_(0.01572126444976191365504766913460853166159) }}, 
      {{ SC_(1.47850000858306884765625), SC_(0.01619781384600798034797161476617322052959) }}, 
      {{ SC_(1.47899997234344482421875), SC_(0.01667414829492702066558118850104769852234) }}, 
      {{ SC_(1.4795000553131103515625), SC_(0.01715038148057877526958196636506777091166) }}, 
      {{ SC_(1.480000019073486328125), SC_(0.01762628654041217983419823745186174695842) }}, 
      {{ SC_(1.4804999828338623046875), SC_(0.01810197720920750584264288401814595315433) }}, 
      {{ SC_(1.48099994659423828125), SC_(0.01857745367191608501032749919254492077255) }}, 
      {{ SC_(1.48150002956390380859375), SC_(0.01905282940736078826428345835701187921344) }}, 
      {{ SC_(1.48199999332427978515625), SC_(0.01952787796086124235035667894483836618612) }}, 
      {{ SC_(1.48249995708465576171875), SC_(0.02000271286178744470899885416201568506841) }}, 
      {{ SC_(1.4830000400543212890625), SC_(0.02047744743549734724595413353304339596842) }}, 
      {{ SC_(1.483500003814697265625), SC_(0.02095185553229931162465762694540459626532) }}, 
      {{ SC_(1.4839999675750732421875), SC_(0.02142605052796892258114323238639681979697) }}, 
      {{ SC_(1.48450005054473876953125), SC_(0.02190014559479680012423194248738781677386) }}, 
      {{ SC_(1.48500001430511474609375), SC_(0.02237391488729442162211914583054893402186) }}, 
      {{ SC_(1.48549997806549072265625), SC_(0.02284747162804102549936129615381098290523) }}, 
      {{ SC_(1.48600006103515625), SC_(0.02332092883665599429697089795154616828867) }}, 
      {{ SC_(1.4865000247955322265625), SC_(0.02379406097107123019383531656762472857036) }}, 
      {{ SC_(1.486999988555908203125), SC_(0.02426698110106619670748667123423481731356) }}, 
      {{ SC_(1.4874999523162841796875), SC_(0.02473968940861605188874507129358473978676) }}, 
      {{ SC_(1.48800003528594970703125), SC_(0.02521229871039613408736493282176431879518) }}, 
      {{ SC_(1.48849999904632568359375), SC_(0.02568458386767842754083514014266512257621) }}, 
      {{ SC_(1.48899996280670166015625), SC_(0.02615665774712770972822278823384134472911) }}, 
      {{ SC_(1.4895000457763671875), SC_(0.02662863301366652729705485156514156921969) }}, 
      {{ SC_(1.4900000095367431640625), SC_(0.02710028483017233972822145485920594582439) }}, 
      {{ SC_(1.490499973297119140625), SC_(0.02757172591142969944611182410222535997026) }}, 
      {{ SC_(1.49100005626678466796875), SC_(0.02804306877099953245333159632753441304838) }}, 
      {{ SC_(1.49150002002716064453125), SC_(0.02851408887259174195648360999928964786226) }}, 
      {{ SC_(1.49199998378753662109375), SC_(0.02898489877950229580989209771050696266479) }}, 
      {{ SC_(1.49249994754791259765625), SC_(0.02945549867145914132386877573515702816588) }}, 
      {{ SC_(1.493000030517578125), SC_(0.02992600086081766434888226036243716094018) }}, 
      {{ SC_(1.4934999942779541015625), SC_(0.03039618121119222213671842099543294002594) }}, 
      {{ SC_(1.493999958038330078125), SC_(0.03086615208450468685425685392189632212515) }}, 
      {{ SC_(1.49450004100799560546875), SC_(0.03133602564265587447963502512063650812183) }}, 
      {{ SC_(1.49500000476837158203125), SC_(0.0318055780482999943002748605694401059607) }}, 
      {{ SC_(1.49549996852874755859375), SC_(0.03227492151277847133068637874480248065187) }}, 
      {{ SC_(1.4960000514984130859375), SC_(0.03274416804792387851799905190703885999517) }}, 
      {{ SC_(1.4965000152587890625), SC_(0.03321309411466675105852178534017282410089) }}, 
      {{ SC_(1.4969999790191650390625), SC_(0.0336818117741548556801742840271873983606) }}, 
      {{ SC_(1.49750006198883056640625), SC_(0.03415043288853741106672780407819814754177) }}, 
      {{ SC_(1.49800002574920654296875), SC_(0.03461873421626057425769027729390348592731) }}, 
      {{ SC_(1.49849998950958251953125), SC_(0.03508682766866380229344336380060751609967) }}, 
      {{ SC_(1.49899995326995849609375), SC_(0.03555471342260717737839815920297809359884) }}, 
      {{ SC_(1.4995000362396240234375), SC_(0.03602250314126230243963242841830489771559) }}
   } };
