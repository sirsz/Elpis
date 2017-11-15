
breed [ staff ]
breed [ arrow direction]

;;******** GLOBAL VARIABLES ***************

globals [ 
  x1 x2 y1 y2 ss
  time  ; time counter (in seconds)
  flag-agents   
  target-heading 
  total
  
  flag-shift
  end-shift

  file file-w 

  
  color-list
  agent-color
  colore-zona-
  soddisfazione
  Average-t-evac       
  
  time-flag
  time-step
  total-flag

  cont-attiv-shift
  flag-attiv-shift  
  rnd-shift

  time-min
  time-max
 
  target-direction
  elimina-turtles-bloccate
  
  cont-ticks-poisson
  ticks-poisson
  
  Ntot
 
  transient
  
  ]


;;******** staff-OWN VARIABLES ***************

staff-own [     
              flag-heading  flag-arrow heading-color last-opera 
              old-patch new-patch old-patch2 new-patch2]


;;******** 'SETUP' PROCEDURE ***************

to setup
 
  ;; (for this model to work with NetLogo's new plotting features,
  ;; __clear-all-and-reset-ticks should be replaced with clear-all at
  ;; the beginning of your setup procedure and reset-ticks at the end
  ;; of the procedure.)
  __clear-all-and-reset-ticks ;; clear all
  
  ;; set initial conditions
  
  set flag-agents (true)
  
  set color-list [15 25 65 85 125 135]
  set agent-color one-of color-list
  set time-min (inter-arrival-time - 10)
  set time-max (inter-arrival-time + 10)  
 
  set elimina-turtles-bloccate true
   
  set colore-zona-fruizione 43
  set n_opere 15
  
  set time-flag true
  set total-flag true

  set cont-attiv-shift 0
  set flag-attiva-shift false 
  set transient 2000
  

  set cont-ticks-poisson 0
  set ticks-poisson random-exponential (inter-arrival-time)  
  set Ntot 0

  
  set-default-shape staff "box"
  set-default-shape arrow "arrow"
  
  ;; THE TOTAL FRUITION SPACE OF ICU IS MADE BY 90 X 90 PATCHES, EACH OF SIDE 0.60 METERS

[import-pcolors "intensywna_terapia_plan.png"] 

  
  set rnd-shift random-normal 200 10
   
  set-current-plot "PDF-Escape-Time"
  set-plot-x-range 0 50
  set-plot-y-range 0 1
    

       
end


;; ************* 'GO' PROCEDURE *******************************

to go
    set time time + 1
    if (not flag-shift and time > shift-time) [set flag-attiva-shift true] 
    if (flag-attiva-shift and not time-flag) [attiva-shift] 
    if (count staff = max-N-visitatori and total-flag) [set total-flag false]
             
    
    if (not flag-shift) [set total (count staff)]
    
    
    plot total
    if (flag-shift and (time mod 5 = 0)) 
    [ask patches with [pcolor = 16 or pcolor = 26] [ifelse (pcolor = 16) [set pcolor 26][set pcolor 16]]]
    
    ifelse (flag-agents) 
    ;; NORMAL STAFF DYNAMICS     
    [       
      ifelse (cont-ticks-poisson > ticks-poisson) 
      [set time-flag true set cont-ticks-poisson 0 set ticks-poisson random-exponential (inter-arrival-time) ] 
      [set cont-ticks-poisson cont-ticks-poisson + 1]
      
      if (time-flag and total-flag)
      [set time-flag false
       ask n-of 1 (patches with [pcolor = red])   
        [set Ntot Ntot + 1
         sprout 1
          [ 
          set breed staff
          set color one-of color-list
        
          set last-opera 44.9
          set heading-color 9.9
          set flag-heading false
          set flag-ingressi true
          set flag-arrow true
          set flag-uscite true
          set old-patch patch-here
          set old-patch2 patch-here
          set heading towardsxy (-30) (30)
          jump 2
          ]  
         ]
        ] 
       set ss 0        
       ask staff  
       [ move set interest
         set ss ss + 1
    ]

     
     ;; EMERGENCY DYNAMICS
     [   
        ask staff ;; 
        [ 
          if (flag-N-new_patient) [set 
         
          set-current-plot-pen "New patient coming"
           
        ]
          
        if count staff = 0 ;; 
        [ 

          stop
        ]  
     ]    
   
      
end



 

;; ************* STANDARD DYNAMICS ROUTINE *******************************

to move
       
      ;; ROUTINE FOR THE ELIMINATION OF TRAPPED TURTLES
      if (time mod 50 = 0 and elimina-turtles-bloccate) [set new-patch patch-here 
                             if ([pcolor] of patch-here != colore-zona-fruizione and new-patch = old-patch) [die] set old-patch patch-here] 
             
      ;; ROUTINE TO AVOID TO REMAIN ON A PATCH ALREADY BUSY
      if ([pcolor] of patch-here = 0 and any? staff-on patch-here) [rt random-normal 0 45 fd 1]  

      ;; ROUTINE TO EXIT FROM ARTWORKS    
      if ([pcolor] of patch-here >= 45 and [pcolor] of patch-here < 46.5) [jump -1]          

      ;; ROUTINE TO AVOID ENTERING INTO ARTWORKS    
      if ([pcolor] of patch-ahead 1 >= 45 and [pcolor] of patch-ahead 1 < 46.5) [jump -1] 
                       
      ;; ROUTINE PER EVITARE DI PENETRARE NELLE PARETI     
      if (([pcolor] of patch-here = 105 and [pcolor] of patch-ahead 1 = 105) or
          ([pcolor] of patch-here = 105.1 and [pcolor] of patch-ahead 1 = 105.1) or
          ([pcolor] of patch-here = 104.9 and [pcolor] of patch-ahead 1 = 104.9))
          [jump -2]       
       
      ;; ROUTINE TO AVOID ENTERING INTO WALLS
      if ([pcolor] of patch-ahead 1 = colore-zona-fruizione) 
         [  
            ifelse (not any? (staff-on patch-ahead 1) with [self != myself]) 
                [ make-1-step ]
                [ ;ROUTINE TO TURN AROUND A BUSY AREA OF USE
                    set scelta2 (random 2)
                    ifelse scelta2 = 0 
                    [rt 90 make-1-step]
                    [lt 90 make-1-step]
                ]
          ]      
          
     ;; ROUTINE FOR THE FRUITION SPACE 
     let neighbcolor [pcolor] of patches in-radius 2 
     if ( [pcolor] of patch-here = colore-zona-fruizione and not member? heading-color neighbcolor
          or [pcolor] of patch-here = colore-zona-fruizione and heading-color = last-opera)
     [jump -2]            
     if ( [pcolor] of patch-here = colore-zona-fruizione and member? heading-color neighbcolor)  
      [ 
            set attesa (attesa + 1)
            set flag-heading false
            if attesa = interesse_t
            [
             set soddisfaz (soddisfaz + interesse_t - cont_impaz)
             set pazienza (random (patience_max - patience_min) + patience_min) 
             set cont_impaz (0)
             set interesse_t (random (interest_max - interest_min) + interest_min) 
             set attesa (0)
             set scelta (0)
             set scelta2 (0)
             set flagcol2 (0)
            
             set last-opera heading-color
             set flag-heading true 
             jump -2 
            ]
      ]            
           
      ;; ROUTINE TO SAVE THE PASSAGE THROUGH A DOOR
      if (flag-ingressi)
      [
      let phcolor ([pcolor] of patch-here)  
      if (phcolor >= 9.0 and phcolor < 10 ) 
          [ ifelse (member? phcolor memoria-ingressi) []
            [ 
              set memoria-ingressi lput phcolor memoria-ingressi
              if (phcolor = 9.8) [if (not member? 46.2 memoria-opere) [set memoria-opere lput 46.2 memoria-opere]] 
              set flag-heading true set flag-ingressi false 
              let target one-of patches in-cone 5 90 with [pcolor = black] 
              set heading towards target
              fd distance target 
            ]
          ] 
      ]   
      
      ;; ROUTINE TO CHOOSE THE GENERAL DIRECTION TO AN ACCESS OR AN EMERGENCY EXIT
     if (flag-heading)
      [  
          let hcolor heading-color
          let target patches in-radius 7 with [pcolor >= 45.0 and pcolor < 46.5 and pcolor != hcolor] 
          if any? target 
          [ 
            let temp memoria-opere
            foreach sort target
            [
             if flag-heading
             [let tcolor 0 ask ? [set tcolor pcolor]
             ifelse (member? tcolor temp or tcolor < max temp) 
              []
              [set heading-color tcolor set flag-heading false]]
            ]  
           ]
          set flag-ingressi true             
       ]       
      if (flag-heading and flag-ingressi)
      [  
          let target patches in-radius 12 with [pcolor > 9.0 and pcolor < 10]        
          if any? target
          [ 
            let temp memoria-ingressi
            foreach sort target
            [
             if flag-heading 
             [let tcolor 0 ask ? [set tcolor pcolor]
             ifelse (member? tcolor temp or precision (tcolor - max temp) 1 > 0.1) 
              []
              [set heading-color tcolor set flag-heading false]]
            ]           
          ]
      ]
      if (flag-heading and heading-color = 46.4)     
      [  ;ROUTINE FOR EXIT 
          let target patches with [pcolor = red ]        
          let tcolor ([pcolor] of one-of target)  
          set heading-color tcolor set flag-heading false
      ] 
      let hcolor heading-color  
      let ttarget one-of patches with [pcolor = hcolor] 
      if (ttarget != patch-here) [set heading towards ttarget]             
      if (any? patches in-radius 1.5 with [pcolor = red] and soddisfaz != 0) [ die ]
      
      ;; WAITING ROUTINE 
      let neighbcolor2 [pcolor] of patches in-radius 4            
      if ([pcolor] of patch-here != colore-zona-fruizione and member? heading-color neighbcolor2) 
      [
        set cont_impaz (cont_impaz + 1)
          if (cont_impaz = pazienza)
          [
            set flag-heading true 
            let target patches in-radius 5 with [pcolor >= 45.0 and pcolor < 46.5] 
            if any? target [let cccolor 0 
                            ask one-of target [set cccolor pcolor]
                            ifelse (member? cccolor memoria-opere) [] 
                            [set memoria-opere lput cccolor memoria-opere]]                      
            set soddisfaz (soddisfaz - pazienza)
            set pazienza (random (patience_max - patience_min) + patience_min)   
            set cont_impaz 0 ] 
       ]  
                  
           
      ;; ROUTINE FOR ADVANCE ON PATCHES IN THE PRESENCE OF OBSTACLES
      let neighbcolor3 [pcolor] of patches in-radius 2 
      if ([pcolor] of patch-ahead 1 = 0 or 
         ([pcolor] of patch-ahead 1 >= 9 and [pcolor] of patch-ahead 1 < 10) or
         ([pcolor] of patch-here = colore-zona-fruizione and not member? heading-color neighbcolor3 ))            
          [ 
              ifelse (not any? (staff-on patch-ahead 1) with [self != myself] and 
                     ([pcolor] of patch-ahead 1 < 45.0 or [pcolor] of patch-ahead 1 >= 46.5) ) 
                     [ make-1-step ]
                     [ 
                      let scelta3 (random 2)
                      ifelse scelta3 = 0        
                      [ lt -45 ] [ rt -45 ]
                     ]
          ]
          
          
    
          
end


;; ************** ROUTINE FOR MOVING TO THE CENTER OF THE NEXT PATCH *************
to make-1-step   

   let target patch-ahead 1  
   set heading towards target
   fd distance target 

end



;; ************* ACTIVATE shift *******************************
to attiva-shift

if (time-flag = false) 
[
set flag-shift true
ask staff [set flag-heading true set memory []]
set flag-agents false        

set end-shift time
set flag-attiva-shift false
]

end



;; ************* shift DYNAMICS ROUTINE ****************************************************************************************************************

to move-shift

      set tempo_uscita tempo_uscita + 1     
            
      ;; ROUTINE FOR THE ELIMINATION OF TRAPPED TURTLES        
      if (time mod 10 = 0) [set new-patch patch-here 
                             if (new-patch = old-patch and (heading-color = 9.6 or heading-color = 46.1 or heading-color = 55.1)) [set heading-color 9.7] 
                             if (new-patch = old-patch and ([pcolor] of patch-here = 9.9)) [set heading-color 15]                              
                             set old-patch patch-here ]   
      if (time mod 20 = 0 and elimina-turtles-bloccate) [set new-patch2 patch-here 
                             if (new-patch2 = old-patch2) [set v-eliminati v-eliminati + 1 die] set old-patch2 patch-here ]   
                             
            
      ;; ROUTINE TO AVOID ENTERING INTO ARTWORKS     
      if ([pcolor] of patch-here >= 45 and [pcolor] of patch-here < 46.5) [jump -2]
      
      ;; ROUTINE TO AVOID ENTERING INTO WALLS      
      if ([pcolor] of patch-here = 104.9 and [pcolor] of patch-ahead 1 = 104.9) [jump -2]      
      
      ;; Routine to advance when you are on a green emergency exit 
        if ([pcolor] of patch-here = 55.2 ) [ if f flag-trapped false]  jump 3]                            
                                                                                                 
        if ([pcolor] of patch-here = 13.1 ) [jump 3 set heading-color red set flag-heading false] 
        
      if (any? arrow in-radius 10) [set flag-arrow false]      

      ifelse (flag-arrow)  
      [         
      ;; ROUTINE TO REMOVE AN ENTRY FROM MEMORY
      let phcolor ([pcolor] of patch-here)  
      if (phcolor >= 8.8 and phcolor < 10) 
          [ 
            jump 3 set flag-heading true 
            set memoria-ingressi remove phcolor memoria-ingressi
          ]           
      ]
      [     
      ;; ROUTINE TO SAVE THE PASSAGE THROUGH A DOOR
      let phcolor ([pcolor] of patch-here)  
      if (phcolor >= 9.0 and phcolor < 10 ) 
          [ ifelse (member? phcolor memoria-ingressi) []
            [ jump 3 set flag-heading true 
              set memoria-ingressi lput phcolor memoria-ingressi]
          ] 
      ]
      

           
      ;; ROUTINE TO CHOOSE THE GENERAL DIRECTION TO AN ACCESS or AN EMERGENCY EXIT
      
      if ([pcolor] of patch-here = 5 )
       [  ;Routine da eseguire se ci si trova su patch grigie
           let target patches with [pcolor = 13.1]        
           if any? target 
           [ let tcolor ([pcolor] of one-of target)  
             set heading-color tcolor 
            ]
        ]      
      
      
      if (flag-heading)
       [  ;CHOOSE ONE OF THE EMERGENCY EXITS WITHIN THE RADIUS OF VISION
          let target patches in-radius 12 with [pcolor = 55.1 ]    ;or pcolor = red    
           if any? target 
           [ let tcolor ([pcolor] of one-of target)  
             set heading-color tcolor set flag-heading false
            ]
        ]     
   
   if (flag-heading)
       [  ;CHOOSE ONE OF THE EMERGENCY EXITS WITHIN THE RADIUS OF VISION
          let target patches in-radius 15 with [pcolor = red]        
           if any? target 
           [ let tcolor ([pcolor] of one-of target)  
             set heading-color tcolor set flag-heading false
            ]
        ]     
   
     
     
      
      if (flag-heading and not flag-arrow) 
      [  ;CHOOSE ONE OF THE ENTRANCES WITHIN THE RADIUS OF VISION
          let target patches in-radius 20 with [pcolor >= 9.0 and pcolor < 10 ]        
          if any? target 
        [ let tcolor ([pcolor] of one-of target)  
            ifelse (member? tcolor memoria-ingressi) []
              [set heading-color tcolor set flag-heading false ]
           ]
      ]
  
      ;CHECK IF THERE IS IN FRONT OF A WALL WHILE ATTEMPTING AN ACCESS
       if (flag-heading = false and ( (heading-color > 54 and heading-color < 56) or heading-color = 15))
      [
        let target patches in-cone 6 40 with [pcolor = 104.9 or pcolor = 105]   
        if any? target [set flag-heading true]
      ]
      
    
           
      if (flag-heading and (length memory = 0)) [set heading-color 9.0]   
      let hcolor heading-color  
      let ttarget one-of patches with [pcolor = hcolor] 
      if (ttarget != patch-here) [set heading towards ttarget]             
      if (any? patches in-radius 1 with [pcolor = red]) [set lmove]
      
;; ROUTINE FOR ADVANCE ON PATCHES IN THE PRESENCE OF OBSTACLES/other Agents      
      ifelse (([pcolor] of patch-ahead 1 < 104.9 or [pcolor] of patch-ahead 1 > 105.1)
              and ([pcolor] of patch-ahead 1 < 45 or [pcolor] of patch-ahead 1 >= 46.5))
         [ 
             ifelse (not any? (staff-on patch-ahead 1) with [self != myself]) [ make-1-step ]
                [ 
                   let scelta3 (random 2)
                   ifelse scelta3 = 0        
                   [ lt 45 ] [ rt 45 ]
                ]
          ]
          [ set heading random 360  
            if (([pcolor] of patch-ahead 1 < 104.9 or [pcolor] of patch-ahead 1 > 105.1) and
               ([pcolor] of patch-ahead 1 < 45 or [pcolor] of patch-ahead 1 >= 46.5))
               [ make-1-step ] ]          
               
           
end


@#$#@#$#@
GRAPHICS-WINDOW
463
11
1183
752
45
45
7.8022
1
10
1
1
1
0
0
0
1
-45
45
-45
45
0
0
1
ticks
30.0

BUTTON
140
10
240
47
GO
go
T
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

BUTTON
20
10
141
47
SETUP
setup
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

PLOT
20
590
460
753
PDF-Escape-Time
escape time
# of visitors
0.0
50.0
0.0
50.0
true
false
"" ""
PENS
"default" 2.0 1 -2674135 true "" ""

BUTTON
242
10
459
44
End of perosnel shift
attiva-allarme
NIL
1
T
OBSERVER
NIL
NIL
NIL
NIL
1

@#$#@#$#@

THE MODEL

This NetLogo model allows to perform an agent-based simulation of people moving in a closed spatial environment. The case study is the ground floor of the Castello Ursino mu-seum in Catania (Italy), a quite complex old building built in 1250 as one of the royal castles of Emperor Frederick II. The simulation allows to evaluate the so called “carrying capacity” of the museum, i.e. the total number of visitors the building can tolerate keeping maximum the level of global satisfaction and taking into account the safety of the building, in terms of evacuation strategies and performances. 

The Castello Ursino is a square compact building, with four towers and ten rooms placed around a central courtyard. This building represents a good environment to test the complex emerging behaviour of a great number of visitors walking along its rooms full of paintings, sculptures and other works of art, trying to maximize their personal satisfaction during their visit experience, while interacting each other and with the envi-ronment. There is only one main entrance/exit door, but many rooms which contain differ-ent paintings and artworks, thus providing different levels of interest to visitors. 

The planimetry of the ground floor of Castello Ursino museum has been reproduced within the NetLogo interface preserving the real scale. The grain of resolution is a square patch 60cm x 60cm, able to carry one visitor at a time. The different patch’s colours of the spatial environment allow the agents to recognize and properly interact with objects with different “physical property”, i.e. internal and external walls (in blue), external entrance/exit door (red on the bottom), internal doors (white), three emergency exits (green) from rooms 2, 4 and 8 to the internal courtyard (gray), one intermediate emergency exit (dark red) from the courtyard to room 10, artworks (yellow rectangular areas arbitrarily distributed inside the rooms), fruition spaces around the artworks (light yellow) and free space (in black). The green arrows in rooms 6, 7 and 10 represents emergency signs indicating the closer exit direction.

NORMAL FRUITION DYNAMICS

In the normal fruition dynamics, visitors access the museum with a Poisson arrival, i.e. with exponentially distributed inter-arrival times ΔT (with a given mean <ΔT >, expressed in seconds and fixed through the correspondent slider within the NetLogo Interface). Visitors move counter-clockwise along one patch in one time-step (1 time step=1s). The motion of the visitors is not predetermined and follows a few intuitive rules: each agent possesses a radius of vision and is attracted by doors or artworks around him; when no obstacles are present, he moves to-wards his target, doors or artworks, which are dynamically stored in an individual memory so that he will not pass twice through the same door and will not attend twice the same artwork; in presence of obstacles, walls or other people, he tries to avoid them going around in a random direction. 

For each artwork, each agent has a different interest, i.e. the time he is willing to spend enjoying that artwork, and a different patience, i.e. the time he is willing to spend waiting for free space in the fruition area around that artwork before switching to the next artwork. These values are randomly extracted with uniform probability, for every visitor and for every artwork, within the intervals [Imin=20s, Imax=50s] (for the interest) and [Pmin=30s, Pmax=40s] (for the patience). These values are, of course, arbitrary, but plausible for our case study. In any case, they can be changed through the correspondent sliders within the Interface. 

As a function of both the patience and the interest it is possible to show in real time the level of satisfaction of each visitor: in particular, individual satisfaction S(i) increases with the number of artworks really visited (proportionally to the interest) and decreases with the time spent in the fruition area for every artwork (proportionally to the patience), which in turn is affected by the presence of the other visitors. Of course S(i) grows or decreases as function of time until the visitor i-th is still inside the museum. Once the visit is over, visitors exit from the only external door in room 10, which is the same used to access the museum, and their final satisfaction is added to a global variable, called ‘total satisfaction’. The latter will be divided, in turn, by the number of visitors involved, in order to calculate our main variable, i.e. the ‘average satisfaction’ shown in the correspondent plot.

Before start the simulation, it is possible to fix the desired maximum number of visitors who have to be simultaneously present in the museum through the correspondent input box, then press the button SETUP. By pressing the button START the simulation goes on and the TIMER starts to report the global simulation time (in seconds). At the same time, the plots start to show both the actual number of visitor inside the museum and the real-time average satisfaction. 

EMERGENCY DYNAMICS

Let us focus on what happens when, after a transient regime of normal fruition of the museum, an alarm situation suddenly arises. In such a situation, of course, the behaviour dynamics of the agents changes. It is possible to choose the alarm-time through the correspondent input box at the beginning of the simulation or to activate the alarm whatever one wants during the simulation by pressing the correspondent button. It is also possible to choose the configuration of emergency exits (with three, two or only one exits) by means of the correspondent “chooser” menù.
  
Actually, as soon as an emergency alarm goes off, each visitors checks if there is one of the emergency exits overlooking the central courtyard within his radius of vision. If so, he moves towards that direction, otherwise, if emergency signs are present and visible to him, he follows them; if not, he goes backward following the same path where he came from, until the external entrance/exit door is found. Of course, visitors gathered in the courtyard, rapidly make their way towards the intermediate emergency exit then, finally, they reach the exit door. 

The real-time number of people crossing the emergency exits is reported in the correspondent plot. Furthermore, at the end of the simulation, i.e. when all the visitors leaved the museum, the frequency distribution of the single visitors escape times and the total evacuation time are also reported.
@#$#@#$#@
default
true
0
Polygon -7500403 true true 150 5 40 250 150 205 260 250

ant
true
0
Polygon -7500403 true true 136 61 129 46 144 30 119 45 124 60 114 82 97 37 132 10 93 36 111 84 127 105 172 105 189 84 208 35 171 11 202 35 204 37 186 82 177 60 180 44 159 32 170 44 165 60
Polygon -7500403 true true 150 95 135 103 139 117 125 149 137 180 135 196 150 204 166 195 161 180 174 150 158 116 164 102
Polygon -7500403 true true 149 186 128 197 114 232 134 270 149 282 166 270 185 232 171 195 149 186
Polygon -7500403 true true 225 66 230 107 159 122 161 127 234 111 236 106
Polygon -7500403 true true 78 58 99 116 139 123 137 128 95 119
Polygon -7500403 true true 48 103 90 147 129 147 130 151 86 151
Polygon -7500403 true true 65 224 92 171 134 160 135 164 95 175
Polygon -7500403 true true 235 222 210 170 163 162 161 166 208 174
Polygon -7500403 true true 249 107 211 147 168 147 168 150 213 150

arrow
true
0
Polygon -7500403 true true 150 0 0 150 105 150 105 293 195 293 195 150 300 150

bee
true
0
Polygon -1184463 true false 152 149 77 163 67 195 67 211 74 234 85 252 100 264 116 276 134 286 151 300 167 285 182 278 206 260 220 242 226 218 226 195 222 166
Polygon -16777216 true false 150 149 128 151 114 151 98 145 80 122 80 103 81 83 95 67 117 58 141 54 151 53 177 55 195 66 207 82 211 94 211 116 204 139 189 149 171 152
Polygon -7500403 true true 151 54 119 59 96 60 81 50 78 39 87 25 103 18 115 23 121 13 150 1 180 14 189 23 197 17 210 19 222 30 222 44 212 57 192 58
Polygon -16777216 true false 70 185 74 171 223 172 224 186
Polygon -16777216 true false 67 211 71 226 224 226 225 211 67 211
Polygon -16777216 true false 91 257 106 269 195 269 211 255
Line -1 false 144 100 70 87
Line -1 false 70 87 45 87
Line -1 false 45 86 26 97
Line -1 false 26 96 22 115
Line -1 false 22 115 25 130
Line -1 false 26 131 37 141
Line -1 false 37 141 55 144
Line -1 false 55 143 143 101
Line -1 false 141 100 227 138
Line -1 false 227 138 241 137
Line -1 false 241 137 249 129
Line -1 false 249 129 254 110
Line -1 false 253 108 248 97
Line -1 false 249 95 235 82
Line -1 false 235 82 144 100

bird1
false
0
Polygon -7500403 true true 2 6 2 39 270 298 297 298 299 271 187 160 279 75 276 22 100 67 31 0

bird2
false
0
Polygon -7500403 true true 2 4 33 4 298 270 298 298 272 298 155 184 117 289 61 295 61 105 0 43

boat1
false
0
Polygon -1 true false 63 162 90 207 223 207 290 162
Rectangle -6459832 true false 150 32 157 162
Polygon -13345367 true false 150 34 131 49 145 47 147 48 149 49
Polygon -7500403 true true 158 33 230 157 182 150 169 151 157 156
Polygon -7500403 true true 149 55 88 143 103 139 111 136 117 139 126 145 130 147 139 147 146 146 149 55

boat2
false
0
Polygon -1 true false 63 162 90 207 223 207 290 162
Rectangle -6459832 true false 150 32 157 162
Polygon -13345367 true false 150 34 131 49 145 47 147 48 149 49
Polygon -7500403 true true 157 54 175 79 174 96 185 102 178 112 194 124 196 131 190 139 192 146 211 151 216 154 157 154
Polygon -7500403 true true 150 74 146 91 139 99 143 114 141 123 137 126 131 129 132 139 142 136 126 142 119 147 148 147

boat3
false
0
Polygon -1 true false 63 162 90 207 223 207 290 162
Rectangle -6459832 true false 150 32 157 162
Polygon -13345367 true false 150 34 131 49 145 47 147 48 149 49
Polygon -7500403 true true 158 37 172 45 188 59 202 79 217 109 220 130 218 147 204 156 158 156 161 142 170 123 170 102 169 88 165 62
Polygon -7500403 true true 149 66 142 78 139 96 141 111 146 139 148 147 110 147 113 131 118 106 126 71

box
false
0
Polygon -7500403 true true 45 255 255 255 255 45 45 45

butterfly1
true
0
Polygon -16777216 true false 151 76 138 91 138 284 150 296 162 286 162 91
Polygon -7500403 true true 164 106 184 79 205 61 236 48 259 53 279 86 287 119 289 158 278 177 256 182 164 181
Polygon -7500403 true true 136 110 119 82 110 71 85 61 59 48 36 56 17 88 6 115 2 147 15 178 134 178
Polygon -7500403 true true 46 181 28 227 50 255 77 273 112 283 135 274 135 180
Polygon -7500403 true true 165 185 254 184 272 224 255 251 236 267 191 283 164 276
Line -7500403 true 167 47 159 82
Line -7500403 true 136 47 145 81
Circle -7500403 true true 165 45 8
Circle -7500403 true true 134 45 6
Circle -7500403 true true 133 44 7
Circle -7500403 true true 133 43 8

circle
false
0
Circle -7500403 true true 35 35 230

link
true
0
Line -7500403 true 150 0 150 300

link direction
true
0
Line -7500403 true 150 150 30 225
Line -7500403 true 150 150 270 225

person
false
4
Circle -7500403 true false 155 20 63
Rectangle -7500403 true false 158 79 217 164
Polygon -7500403 true false 158 81 110 129 131 143 158 109 165 110
Polygon -7500403 true false 216 83 267 123 248 143 215 107
Polygon -7500403 true false 167 163 145 234 183 234 183 163
Polygon -7500403 true false 195 163 195 233 227 233 206 159
Circle -1184463 true true 150 14 67
Rectangle -1184463 true true 158 78 215 164
Polygon -1184463 true true 156 81 110 129 130 142 160 106
Polygon -1184463 true true 215 83 214 111 246 141 266 121
Polygon -1184463 true true 167 164 145 232 180 233 183 159
Polygon -1184463 true true 193 164 196 230 227 231 208 159 193 164

sheep
false
15
Rectangle -1 true true 90 75 270 225
Circle -1 true true 15 75 150
Rectangle -16777216 true false 81 225 134 286
Rectangle -16777216 true false 180 225 238 285
Circle -16777216 true false 1 88 92

spacecraft
true
0
Polygon -7500403 true true 150 0 180 135 255 255 225 240 150 180 75 240 45 255 120 135

thin-arrow
true
0
Polygon -7500403 true true 150 0 0 150 120 150 120 293 180 293 180 150 300 150

truck-down
false
0
Polygon -7500403 true true 225 30 225 270 120 270 105 210 60 180 45 30 105 60 105 30
Polygon -8630108 true false 195 75 195 120 240 120 240 75
Polygon -8630108 true false 195 225 195 180 240 180 240 225

truck-left
false
0
Polygon -7500403 true true 120 135 225 135 225 210 75 210 75 165 105 165
Polygon -8630108 true false 90 210 105 225 120 210
Polygon -8630108 true false 180 210 195 225 210 210

truck-right
false
0
Polygon -7500403 true true 180 135 75 135 75 210 225 210 225 165 195 165
Polygon -8630108 true false 210 210 195 225 180 210
Polygon -8630108 true false 120 210 105 225 90 210

turtle
true
0
Polygon -7500403 true true 138 75 162 75 165 105 225 105 225 142 195 135 195 187 225 195 225 225 195 217 195 202 105 202 105 217 75 225 75 195 105 187 105 135 75 142 75 105 135 105

wolf
false
0
Rectangle -7500403 true true 15 105 105 165
Rectangle -7500403 true true 45 90 105 105
Polygon -7500403 true true 60 90 83 44 104 90
Polygon -16777216 true false 67 90 82 59 97 89
Rectangle -1 true false 48 93 59 105
Rectangle -16777216 true false 51 96 55 101
Rectangle -16777216 true false 0 121 15 135
Rectangle -16777216 true false 15 136 60 151
Polygon -1 true false 15 136 23 149 31 136
Polygon -1 true false 30 151 37 136 43 151
Rectangle -7500403 true true 105 120 263 195
Rectangle -7500403 true true 108 195 259 201
Rectangle -7500403 true true 114 201 252 210
Rectangle -7500403 true true 120 210 243 214
Rectangle -7500403 true true 115 114 255 120
Rectangle -7500403 true true 128 108 248 114
Rectangle -7500403 true true 150 105 225 108
Rectangle -7500403 true true 132 214 155 270
Rectangle -7500403 true true 110 260 132 270
Rectangle -7500403 true true 210 214 232 270
Rectangle -7500403 true true 189 260 210 270
Line -7500403 true 263 127 281 155
Line -7500403 true 281 155 281 192

wolf-left
false
3
Polygon -6459832 true true 117 97 91 74 66 74 60 85 36 85 38 92 44 97 62 97 81 117 84 134 92 147 109 152 136 144 174 144 174 103 143 103 134 97
Polygon -6459832 true true 87 80 79 55 76 79
Polygon -6459832 true true 81 75 70 58 73 82
Polygon -6459832 true true 99 131 76 152 76 163 96 182 104 182 109 173 102 167 99 173 87 159 104 140
Polygon -6459832 true true 107 138 107 186 98 190 99 196 112 196 115 190
Polygon -6459832 true true 116 140 114 189 105 137
Rectangle -6459832 true true 109 150 114 192
Rectangle -6459832 true true 111 143 116 191
Polygon -6459832 true true 168 106 184 98 205 98 218 115 218 137 186 164 196 176 195 194 178 195 178 183 188 183 169 164 173 144
Polygon -6459832 true true 207 140 200 163 206 175 207 192 193 189 192 177 198 176 185 150
Polygon -6459832 true true 214 134 203 168 192 148
Polygon -6459832 true true 204 151 203 176 193 148
Polygon -6459832 true true 207 103 221 98 236 101 243 115 243 128 256 142 239 143 233 133 225 115 214 114

wolf-right
false
3
Polygon -6459832 true true 170 127 200 93 231 93 237 103 262 103 261 113 253 119 231 119 215 143 213 160 208 173 189 187 169 190 154 190 126 180 106 171 72 171 73 126 122 126 144 123 159 123
Polygon -6459832 true true 201 99 214 69 215 99
Polygon -6459832 true true 207 98 223 71 220 101
Polygon -6459832 true true 184 172 189 234 203 238 203 246 187 247 180 239 171 180
Polygon -6459832 true true 197 174 204 220 218 224 219 234 201 232 195 225 179 179
Polygon -6459832 true true 78 167 95 187 95 208 79 220 92 234 98 235 100 249 81 246 76 241 61 212 65 195 52 170 45 150 44 128 55 121 69 121 81 135
Polygon -6459832 true true 48 143 58 141
Polygon -6459832 true true 46 136 68 137
Polygon -6459832 true true 45 129 35 142 37 159 53 192 47 210 62 238 80 237
Line -16777216 false 74 237 59 213
Line -16777216 false 59 213 59 212
Line -16777216 false 58 211 67 192
Polygon -6459832 true true 38 138 66 149
Polygon -6459832 true true 46 128 33 120 21 118 11 123 3 138 5 160 13 178 9 192 0 199 20 196 25 179 24 161 25 148 45 140
Polygon -6459832 true true 67 122 96 126 63 144

@#$#@#$#@
NetLogo 5.2.0
@#$#@#$#@
@#$#@#$#@
@#$#@#$#@
<experiments>
  <experiment name="experiment1" repetitions="10" runMetricsEveryStep="true">
    <setup>setup</setup>
    <go>go</go>
    <enumeratedValueSet variable="time-max">
      <value value="160"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="group-size">
      <value value="5"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="time-min">
      <value value="140"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="forma-agenti">
      <value value="&quot;box&quot;"/>
    </enumeratedValueSet>
    <enumeratedValueSet variable="agenti">
      <value value="850"/>
    </enumeratedValueSet>
  </experiment>
</experiments>
@#$#@#$#@
@#$#@#$#@
default
0.0
-0.2 0 0.0 1.0
0.0 1 1.0 0.0
0.2 0 0.0 1.0
link direction
true
0
Line -7500403 true 150 150 90 180
Line -7500403 true 150 150 210 180

@#$#@#$#@
0
@#$#@#$#@
