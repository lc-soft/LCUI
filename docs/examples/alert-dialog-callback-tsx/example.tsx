import {
  AlertDialog,
  AlertDialogTrigger,
  AlertDialogOverlay,
  AlertDialogContent,
  AlertDialogHeader,
  AlertDialogTitle,
  AlertDialogFooter,
  AlertDialogCancel,
  AlertDialogAction,
  Text,
  Portal,
} from "@lcui/react";

export default function Example() {
  return (
    <div className="flex flex-col gap-2">
      <AlertDialog>
        <AlertDialogTrigger>Confirm Changes</AlertDialogTrigger>
        <Portal>
          <AlertDialogOverlay>
            <AlertDialogContent>
              <AlertDialogHeader>
                <AlertDialogTitle>Save changes?</AlertDialogTitle>
              </AlertDialogHeader>
              <AlertDialogFooter>
                <AlertDialogCancel variant="outline">Discard</AlertDialogCancel>
                <AlertDialogAction variant="primary">Save</AlertDialogAction>
              </AlertDialogFooter>
            </AlertDialogContent>
          </AlertDialogOverlay>
        </Portal>
      </AlertDialog>
      <Text>No choice yet</Text>
    </div>
  );
}
