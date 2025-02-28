/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#import <Foundation/Foundation.h>

@import IGListDiffKit;

#import "IGListBindingSectionControllerDataSource.h"
#import "IGListBindingSectionControllerSelectionDelegate.h"
#import "IGListSectionController.h"

@protocol IGListDiffable;

@class IGListBindingSectionController;

NS_ASSUME_NONNULL_BEGIN

/**
 This section controller uses a data source to transform its "top level" object into an array of diffable view models.
 It then automatically binds each view model to cells via the `IGListBindable` protocol.

 Models used with `IGListBindingSectionController` should take special care to always return `YES` for identical
 objects. That is, any objects with matching `-diffIdentifier`s should always be equal, that way the section controller
 can create new view models via the data source, create a diff, and update the specific cells that have changed.

 In Objective-C, your `-isEqualToDiffableObject:` can simply be:
 ```
 - (BOOL)isEqualToDiffableObject:(id)object {
   return YES;
 }
 ```

 In Swift:
 ```
 func isEqual(toDiffableObject object: IGListDiffable?) -> Bool {
   return true
 }
 ```

 Only when `-diffIdentifier`s match is object equality compared, so you can assume the class is the same, and the
 instance has already been checked.
 */
NS_SWIFT_NAME(ListBindingSectionController)
@interface IGListBindingSectionController<__covariant ObjectType : id<IGListDiffable>> : IGListSectionController

/**
 A data source that transforms a top-level object into view models, and returns cells and sizes for given view models.
 */
@property (nonatomic, weak, nullable) id<IGListBindingSectionControllerDataSource> dataSource;

/**
 A delegate that receives selection events from cells in an `IGListBindingSectionController` instance.
 */
@property (nonatomic, weak, nullable) id<IGListBindingSectionControllerSelectionDelegate> selectionDelegate;

/**
 The object currently assigned to the section controller, if any.
 */
@property (nonatomic, strong, readonly, nullable) ObjectType object;

/**
 The array of view models created from the data source. Values are changed when the top-level object changes or by
 calling `-updateAnimated:completion:` manually.
 */
@property (nonatomic, copy, readonly) NSArray<id<IGListDiffable>> *viewModels;

/**
 Tells the section controller to query for new view models, diff the changes, and update its cells.

 @param animated A flag indicating if the transition should be animated or not.
 @param completion An optional completion block executed after updates finish. Parameter is YES if updates were applied.
 */
- (void)updateAnimated:(BOOL)animated completion:(nullable void (^)(BOOL updated))completion;

/**
 Notifies the section that a list object should move within a section as the result of interactive reordering.

 @param sourceIndex The starting index of the object.
 @param destinationIndex The ending index of the object.

 @note this method must be implemented if interactive reordering is enabled. To ensure updating the internal viewModels array, **calling super is required**, preferably before your own implementation.
 */
- (void)moveObjectFromIndex:(NSInteger)sourceIndex toIndex:(NSInteger)destinationIndex NS_REQUIRES_SUPER;

@end

NS_ASSUME_NONNULL_END
