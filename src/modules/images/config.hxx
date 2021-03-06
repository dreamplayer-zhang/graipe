/************************************************************************/
/*                                                                      */
/*               Copyright 2008-2017 by Benjamin Seppke                 */
/*       Cognitive Systems Group, University of Hamburg, Germany        */
/*                                                                      */
/*    This file is part of the GrAphical Image Processing Enviroment.   */
/*    The GRAIPE Website may be found at:                               */
/*        https://github.com/bseppke/graipe                             */
/*    Please direct questions, bug reports, and contributions to        */
/*    the GitHub page and use the methods provided there.               */
/*                                                                      */
/*    Permission is hereby granted, free of charge, to any person       */
/*    obtaining a copy of this software and associated documentation    */
/*    files (the "Software"), to deal in the Software without           */
/*    restriction, including without limitation the rights to use,      */
/*    copy, modify, merge, publish, distribute, sublicense, and/or      */
/*    sell copies of the Software, and to permit persons to whom the    */
/*    Software is furnished to do so, subject to the following          */
/*    conditions:                                                       */
/*                                                                      */
/*    The above copyright notice and this permission notice shall be    */
/*    included in all copies or substantial portions of the             */
/*    Software.                                                         */
/*                                                                      */
/*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND    */
/*    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES   */
/*    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND          */
/*    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT       */
/*    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,      */
/*    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      */
/*    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR     */
/*    OTHER DEALINGS IN THE SOFTWARE.                                   */
/*                                                                      */
/************************************************************************/

#include <QtCore/QtGlobal>

/**
 * @addtogroup graipe_images
 * @{
 *
 * @file
 * @brief Configuration file to the images module
 */

/** Global maximum image width **/
#define MAX_IMAGE_WIDTH  20000
/** Global maximum image height **/
#define MAX_IMAGE_HEIGHT 20000

#ifdef GRAIPE_IMAGES_BUILD
	#if (defined(QT_DLL) || defined(QT_SHARED)) && !defined(QT_PLUGIN)
        /** Set GRAIPE_IMAGES_EXPORT to export functions **/
		#define GRAIPE_IMAGES_EXPORT Q_DECL_EXPORT
	#else
        /** Unset GRAIPE_IMAGES_EXPORT **/
		#define GRAIPE_IMAGES_EXPORT
	#endif
#else
    /** Set GRAIPE_IMAGES_EXPORT to import functions **/
	#define GRAIPE_IMAGES_EXPORT Q_DECL_IMPORT
#endif

/**
 * @}
 */
