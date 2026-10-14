import {
  Dialog,
  DialogTrigger,
  DialogOverlay,
  DialogContent,
  DialogHeader,
  DialogTitle,
  DialogDescription,
  Portal,
} from "@lcui/react";

export default function Example() {
  return (
    <Dialog>
      <DialogTrigger>No Close Button</DialogTrigger>
      <Portal>
        <DialogOverlay>
          {/* Build the content subtree without a DialogClose node. */}
          <DialogContent>
            <DialogHeader>
              <DialogTitle>No Close Button</DialogTitle>
              <DialogDescription>
                This dialog doesn't have a close button in the top-right
                corner.
              </DialogDescription>
            </DialogHeader>
          </DialogContent>
        </DialogOverlay>
      </Portal>
    </Dialog>
  );
}
