# Workflow


The lastmaker software starts off with a model of the bone-structure of the foot and the added skin atop. From these it generates a last and for the last it generates a prototype shoe, that can be modified in standard 3D modelling software (to stick to the open-source thought, the use of the software Blender as a modeller is described later on). 

## Setup footmodel 

The footmodel is a parametric model, that was designed from differnt X-ray scans found on the internet. The mdel is parametrized by a hand full of parameters, making it simple to adapt the model to a foot. A quick initialisation dialog uses the shoe-size and additionally the "wide" - "medium" - "small" notion for the width of the foot. It gives a good starting point. For an average foot it might even provide a perfect fit. 

The default model uses 4 measurements: 

  * The length of the foot from heel to the longest toe.
  * The width of the foot at the toe joints.
  * The circumfence at the heel.
  * The circumfence around the span of the foot.

More models will be constructed later on.

If You later on design a shoe for the same foot-model with the same difference between heel and toes, the last model will be exactly the same. Only the shoe-model will change. So there is no need to create a new last or even create new patterns for the upper part of the shoe. 

## Setup shoe 

The setup of the shoe uses 4 measures: 

  * The height of the heel.
  * The height of the bally(?).
  * The angle of the toes.
  * A mixing coefficient that splits the bending of the foot between the ankle and the forefoot. 

The mixing coefficient is used instead of the heel angle, because this factor is what makes a heeled shoe comfortable. A large value for the factor result in unwearable shoes, that are more likely to be found in the proxymity of some weird foot-binding cults. 

There are some presets based on the shoesize. The presets feature a common range of style for made to measure shoes. 

## Walkcycle support setup 

The walkcycle support dialog is used to optimize the rounding and position of the front and back of the shoe. It simulates the walkcycle and calulates the forces on the foot and the joints. Especially for platform shoes the can make a big differennce and supports the smoothness of the walk. 

The dialog also calculates the forces on the ankle joint in case of a [Umknicken]. 

## Setup of the pattern 

The pattern generator uses basic pattern and stretches these over the last. For boots it also uses the leg model, that is setup in the footmodel dialog. 

The templates to choose from are taken from Oskar Belschings standard work on industrial shoe design. For most models a height can be choosen. 

## Postprocessing

The generated last and the shoe-prototype are only prototypes. These are intended to be adapted in any 3D software at hand. 

The exported files contain the last and the raw shoe as well a a cutaway block. 

The cutaway block is intended to reset the shape of the shoe after it was edited. For example, if You use Blenders sculpt-tools, You may end up with a very deformed shape with an irregular upper and lower side. The supporting curvatures may also be gone. To reset these, a boolean operation with the cutaway-block can be done to remove the extrudes extra shapes. 

The patterns for the upper parts of the shoe are exported as scaleable-vector-graphics. These are intended to be used in a laser- or waterjet-cutter. For modification a vector graphic editor like Inkscape can be used. 
