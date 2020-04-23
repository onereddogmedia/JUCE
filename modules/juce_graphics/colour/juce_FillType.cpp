/*
  ==============================================================================

   This file is part of the JUCE 6 technical preview.
   Copyright (c) 2020 - Raw Material Software Limited

   You may use this code under the terms of the GPL v3
   (see www.gnu.org/licenses).

   For this technical preview, this file is not subject to commercial licensing.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

FillType::FillType() noexcept
    : colour (0xff000000)
{
}

FillType::FillType (Colour c) noexcept
    : colour (c)
{
}

FillType::FillType (const ColourGradient& g)
    : colour (0xff000000), gradient (new ColourGradient (g))
{
}

FillType::FillType (ColourGradient&& g)
    : colour (0xff000000), gradient (new ColourGradient (std::move (g)))
{
}

FillType::FillType (const Image& im, const AffineTransform& t) noexcept
    : colour (0xff000000), image (im), transform (t)
{
}

FillType::FillType (const FillType& other)
    : colour (other.colour),
      gradient (createCopyIfNotNull (other.gradient.get())),
      image (other.image),
      transform (other.transform)
{
}

FillType& FillType::operator= (const FillType& other)
{
    if (this != &other)
    {
        colour = other.colour;
        gradient.reset (createCopyIfNotNull (other.gradient.get()));
        image = other.image;
        transform = other.transform;
    }

    return *this;
}

FillType::FillType (FillType&& other) noexcept
    : colour (other.colour),
      gradient (std::move (other.gradient)),
      image (std::move (other.image)),
      transform (other.transform)
{
}

FillType& FillType::operator= (FillType&& other) noexcept
{
    jassert (this != &other); // hopefully the compiler should make this situation impossible!

    colour = other.colour;
    gradient = std::move (other.gradient);
    image = std::move (other.image);
    transform = other.transform;
    return *this;
}

FillType::~FillType() noexcept
{
}

bool FillType::operator== (const FillType& other) const
{
    return colour == other.colour && image == other.image
            && transform == other.transform
            && (gradient == other.gradient
                 || (gradient != nullptr && other.gradient != nullptr && *gradient == *other.gradient));
}

bool FillType::operator!= (const FillType& other) const
{
    return ! operator== (other);
}

void FillType::setColour (Colour newColour) noexcept
{
    gradient.reset();
    image = {};
    colour = newColour;
}

void FillType::setGradient (const ColourGradient& newGradient)
{
    if (gradient != nullptr)
    {
        *gradient = newGradient;
    }
    else
    {
        image = {};
        gradient.reset (new ColourGradient (newGradient));
        colour = Colours::black;
    }
}

void FillType::setTiledImage (const Image& newImage, const AffineTransform& newTransform) noexcept
{
    gradient.reset();
    image = newImage;
    transform = newTransform;
    colour = Colours::black;
}

void FillType::setOpacity (const float newOpacity) noexcept
{
    colour = colour.withAlpha (newOpacity);
}

bool FillType::isInvisible() const noexcept
{
    return colour.isTransparent() || (gradient != nullptr && gradient->isInvisible());
}

FillType FillType::transformed (const AffineTransform& t) const
{
    FillType f (*this);
    f.transform = f.transform.followedBy (t);
    return f;
}

} // namespace juce
