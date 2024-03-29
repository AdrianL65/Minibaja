# Minibaja
The team participates in national and international SAE events where the mechanical design and the vehicle’s performance is evaluated by undergoing a series of on-site tests, such as obstacle courses. I have been a member of the team since august 2019 where my main role was addressing the vehicle’s steering system. Initially my main contribution was noticing that the vehicle had difficulty steering when a tight turn was involved. After some research I came to the conclusion that the ackermann angle had been calculated inaccurately and what had resulted was an anti-ackermann configuration in the steering geometry. The solution was to simply correct the geometry to a positive Ackermann configuration resulting a smaller turning radius. 

During the period of  Jan-2022 and Jun-2022 I became the vice president and founded alternative branch (Telemetry) with the objective of collecting data from the subsystems of the vehicle via wireless sensory systems; therefore, correlating the collected information with the simulated designs and preforming modifications accordingly. 

The team had the goal to create a method to read the vehicle’s average velocity in real time. To attend this need a non-mounted system was created consisting of three photoelectric sensors mounted to camera like standing structures (made in house), RF modules (E31-230T33D), a Bluetooth module, rechargeable lithium batteries, a microcontroller (KL25Z) and the MIT Appinventor platform. So the way the system works is that the sensors act like invisible ‘start’ and a ‘finish’ lines which have a fixed distance between each other; once the vehicle crosses the start line a timer inside the microcontroller is activated and when the vehicle crosses the finish line the timer stops. Since the time is registered and the distance was fixed, then an estimated velocity can be calculated. The resulting information is communicated to the RF transmitter via a UART protocol. On the other end the RF receiver collects this data (evidently wirelessly) and communicates the information the Bluetooth module via serial communication. Finally an android device with the designed app installed, connects to the Bluetooth module and the real time timer can be visualized. 
My main role during this process was to program the microcontroller to control the sensors into acting like a ‘start’ and a ‘finish’ line, and programming the interrupts necessary for the timer to be created. My second role was to design the simple android app that would act as the timer’s visualizer.

You can find follow the team's intagram @madramsgda_itesm

# Images

![](IMG_20191026_155125453 (1).jpg)

![](IMG_20200131_170529164.jpg)

![](IMG_20220326_145558.jpg)

![](IMG_20201105_113409290.jpg)

![](IMG_20220330_145817.jpg)

