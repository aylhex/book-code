.class Lcom/demo/repackage/MainActivity$1;
.super Ljava/lang/Object;
.source "MainActivity.java"

# interfaces
.implements Landroid/content/DialogInterface$OnClickListener;


# annotations
.annotation system Ldalvik/annotation/EnclosingMethod;
    value = Lcom/demo/repackage/MainActivity;->onCreate(Landroid/os/Bundle;)V
.end annotation

.annotation system Ldalvik/annotation/InnerClass;
    accessFlags = 0x0
    name = null
.end annotation


# instance fields
.field final synthetic this$0:Lcom/demo/repackage/MainActivity;


# direct methods
.method constructor <init>(Lcom/demo/repackage/MainActivity;)V
    .locals 0

    .line 43
    iput-object p1, p0, Lcom/demo/repackage/MainActivity$1;->this$0:Lcom/demo/repackage/MainActivity;

    invoke-direct {p0}, Ljava/lang/Object;-><init>()V

    return-void
.end method


# virtual methods
.method public onClick(Landroid/content/DialogInterface;I)V
    .locals 0

    const/4 p1, 0x0

    .line 47
    invoke-static {p1}, Ljava/lang/System;->exit(I)V

    return-void
.end method
