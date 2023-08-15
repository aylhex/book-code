.class Lcom/demo/repackage/SecondFragment$1;
.super Ljava/lang/Object;
.source "SecondFragment.java"

# interfaces
.implements Landroid/view/View$OnClickListener;


# annotations
.annotation system Ldalvik/annotation/EnclosingMethod;
    value = Lcom/demo/repackage/SecondFragment;->onViewCreated(Landroid/view/View;Landroid/os/Bundle;)V
.end annotation

.annotation system Ldalvik/annotation/InnerClass;
    accessFlags = 0x0
    name = null
.end annotation


# instance fields
.field final synthetic this$0:Lcom/demo/repackage/SecondFragment;


# direct methods
.method constructor <init>(Lcom/demo/repackage/SecondFragment;)V
    .locals 0

    .line 32
    iput-object p1, p0, Lcom/demo/repackage/SecondFragment$1;->this$0:Lcom/demo/repackage/SecondFragment;

    invoke-direct {p0}, Ljava/lang/Object;-><init>()V

    return-void
.end method


# virtual methods
.method public onClick(Landroid/view/View;)V
    .locals 1

    .line 35
    iget-object p1, p0, Lcom/demo/repackage/SecondFragment$1;->this$0:Lcom/demo/repackage/SecondFragment;

    invoke-static {p1}, Landroidx/navigation/fragment/NavHostFragment;->findNavController(Landroidx/fragment/app/Fragment;)Landroidx/navigation/NavController;

    move-result-object p1

    const v0, 0x7f080033

    .line 36
    invoke-virtual {p1, v0}, Landroidx/navigation/NavController;->navigate(I)V

    return-void
.end method
